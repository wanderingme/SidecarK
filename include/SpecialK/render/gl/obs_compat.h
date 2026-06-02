/**
 * obs_compat.h — OBS/graphics-hook compatibility helpers for SidecarK.
 *
 * Provides:
 *   - OBS / foreign-hook detection (module scan + window scan)
 *   - Debug isolation modes via environment variables
 *   - OBS-safe mode state (stable-frame counter, skip reasons)
 *   - SKF1 header validation helpers
 *   - Thread-safe pending-overlay-state model
 *
 * Design rules:
 *   - All detection is one-shot and cached (never called per-frame after init).
 *   - All render-hook state lives in the render thread; no IPC from command thread.
 *   - No blocking calls; try-lock only in the render hook.
 */

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <tlhelp32.h>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwctype>

// ---------------------------------------------------------------------------
// Logging helper — writes to %TEMP%\SidecarK_Overlay.log when diagnostics are
// enabled.  Call SidecarK_OBSCompat_SetDiagCallback() once at startup.
// ---------------------------------------------------------------------------
namespace SidecarK_OBSCompat {

using LogFn = void (*)(const wchar_t* fmt, ...);
static inline LogFn g_log_fn = nullptr;

inline void SetLogFn(LogFn fn) { g_log_fn = fn; }

inline void Log(const wchar_t* fmt, ...)
{
  if (!g_log_fn) return;
  va_list ap;
  va_start(ap, fmt);
  // Forward via a local buffer — LogFn already accepts va_list-style via fmt
  wchar_t buf[512]{};
  _vsnwprintf_s(buf, _TRUNCATE, fmt, ap);
  va_end(ap);
  // Call the provided sink with a pre-formatted string (no extra args)
  g_log_fn(L"%ls", buf);
}

// ---------------------------------------------------------------------------
// PART 2 — Debug isolation modes (read once, cached)
// ---------------------------------------------------------------------------
struct DebugModes
{
  bool no_inject               = false;  // VIRULE_SIDECAR_NO_INJECT
  bool hook_passthrough        = false;  // VIRULE_SIDECAR_HOOK_PASSTHROUGH
  bool no_command_effects      = false;  // VIRULE_SIDECAR_NO_COMMAND_EFFECTS
  bool no_shared_memory_read   = false;  // VIRULE_SIDECAR_NO_SHARED_MEMORY_READ
  bool no_blit                 = false;  // VIRULE_SIDECAR_NO_BLIT
  bool initialized             = false;
};

inline DebugModes& GetDebugModes()
{
  static DebugModes s;
  if (!s.initialized)
  {
    s.initialized = true;
    char buf[4]{};
    auto chk = [&](const char* name) -> bool {
      DWORD n = GetEnvironmentVariableA(name, buf, sizeof(buf));
      return n > 0 && buf[0] == '1';
    };
    s.no_inject             = chk("VIRULE_SIDECAR_NO_INJECT");
    s.hook_passthrough      = chk("VIRULE_SIDECAR_HOOK_PASSTHROUGH");
    s.no_command_effects    = chk("VIRULE_SIDECAR_NO_COMMAND_EFFECTS");
    s.no_shared_memory_read = chk("VIRULE_SIDECAR_NO_SHARED_MEMORY_READ");
    s.no_blit               = chk("VIRULE_SIDECAR_NO_BLIT");

    if (s.hook_passthrough)
      Log(L"sidecar_debug_mode: HOOK_PASSTHROUGH");
    if (s.no_blit)
      Log(L"sidecar_debug_mode: NO_BLIT");
    if (s.no_shared_memory_read)
      Log(L"sidecar_debug_mode: NO_SHARED_MEMORY_READ");
    if (s.no_command_effects)
      Log(L"sidecar_debug_mode: NO_COMMAND_EFFECTS");
    if (s.no_inject)
      Log(L"sidecar_debug_mode: NO_INJECT");
  }
  return s;
}

// ---------------------------------------------------------------------------
// PART 1 — OBS / foreign-hook detection
// ---------------------------------------------------------------------------
struct OBSDetectResult
{
  bool        detected  = false;
  bool        logged    = false;
  wchar_t     modules[512]{};
  wchar_t     windows[256]{};
};

// Returns true if the narrow string |haystack| contains |needle| (case-insensitive).
static inline bool ContainsI(const wchar_t* haystack, const wchar_t* needle)
{
  if (!haystack || !needle) return false;
  // Manual case-insensitive wide-string search
  const size_t nlen = wcslen(needle);
  const size_t hlen = wcslen(haystack);
  if (nlen == 0) return true;
  if (hlen < nlen) return false;
  for (size_t i = 0; i <= hlen - nlen; ++i)
  {
    bool match = true;
    for (size_t j = 0; j < nlen; ++j)
    {
      if (towlower(haystack[i + j]) != towlower(needle[j]))
      { match = false; break; }
    }
    if (match) return true;
  }
  return false;
}

struct WindowScanCtx
{
  bool   found;
  wchar_t names[256];
};

static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lp)
{
  auto* ctx = reinterpret_cast<WindowScanCtx*>(lp);

  wchar_t cls[128]{};
  wchar_t ttl[128]{};
  GetClassNameW(hwnd, cls, (int)_countof(cls));
  GetWindowTextW(hwnd, ttl, (int)_countof(ttl));

  static const wchar_t* const kPats[] = {
    L"graphics_hook_gl_dummy_window",
    L"graphics_hook_dx_dummy_window",
    L"OBS",
    L"capture",
    nullptr
  };

  for (int i = 0; kPats[i]; ++i)
  {
    if (ContainsI(cls, kPats[i]) || ContainsI(ttl, kPats[i]))
    {
      ctx->found = true;
      // Append to names (truncated)
      const size_t cur = wcslen(ctx->names);
      if (cur + 64 < _countof(ctx->names))
      {
        wchar_t entry[64]{};
        _snwprintf_s(entry, _TRUNCATE, L"%ls|", cls[0] ? cls : ttl);
        wcsncat_s(ctx->names, _countof(ctx->names) - cur - 1, entry, _TRUNCATE);
      }
    }
  }
  return TRUE;
}

inline OBSDetectResult& DetectOBS()
{
  static OBSDetectResult s;
  if (s.logged)
    return s;

  // --- Module scan ---
  static const wchar_t* const kModPats[] = {
    L"obs", L"graphics-hook", L"game-capture", L"capture", nullptr
  };

  HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,
                                          GetCurrentProcessId());
  if (snap != INVALID_HANDLE_VALUE)
  {
    MODULEENTRY32W me{};
    me.dwSize = sizeof(me);
    if (Module32FirstW(snap, &me))
    {
      do {
        for (int i = 0; kModPats[i]; ++i)
        {
          if (ContainsI(me.szModule, kModPats[i]))
          {
            s.detected = true;
            const size_t cur = wcslen(s.modules);
            if (cur + 64 < _countof(s.modules))
            {
              wchar_t entry[64]{};
              _snwprintf_s(entry, _TRUNCATE, L"%ls|", me.szModule);
              wcsncat_s(s.modules, _countof(s.modules) - cur - 1, entry, _TRUNCATE);
            }
          }
        }
      } while (Module32NextW(snap, &me));
    }
    CloseHandle(snap);
  }

  // --- Window scan ---
  WindowScanCtx wctx{};
  EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&wctx));
  if (wctx.found)
  {
    s.detected = true;
    wcsncpy_s(s.windows, _countof(s.windows), wctx.names, _TRUNCATE);
  }

  s.logged = true;

  if (s.detected)
  {
    Log(L"sidecar_obs_detect: obs_detected=1 modules=[%ls] windows=[%ls]",
        s.modules[0] ? s.modules : L"(none)",
        s.windows[0] ? s.windows : L"(none)");
    Log(L"sidecar_obs_detect: compat_mode=obs_game_capture_safe");
  }
  else
  {
    Log(L"sidecar_obs_detect: obs_detected=0");
  }

  return s;
}

// ---------------------------------------------------------------------------
// PART 8/9 — OBS-safe mode: stable-frame tracking
// ---------------------------------------------------------------------------
struct ObsSafeState
{
  // Render-target stability (Part 9)
  uint32_t   candidate_w         = 0;
  uint32_t   candidate_h         = 0;
  int        candidate_frames    = 0;
  ULONGLONG  candidate_first_ms  = 0;

  bool       target_stable       = false;
  uint32_t   stable_w            = 0;
  uint32_t   stable_h            = 0;

  // OBS-safe draw delay: require kObsStableFrames in obs mode
  static constexpr int kObsStableFrames = 5;
  int  stable_frames_seen        = 0;
  bool draw_allowed              = false;
  bool draw_allowed_logged       = false;

  // Called each render frame with current client rect dimensions.
  // Returns true when the render target is considered stable.
  bool ObserveSize(uint32_t w, uint32_t h, const OBSDetectResult& obs, LogFn log)
  {
    if (w == 0 || h == 0)
      return false;

    if (w == candidate_w && h == candidate_h)
    {
      ++candidate_frames;
      const ULONGLONG now = GetTickCount64();
      const bool stable_by_frames = (candidate_frames >= kObsStableFrames);
      const bool stable_by_time   = (now - candidate_first_ms >= 500ull);

      if ((stable_by_frames || stable_by_time) && !target_stable)
      {
        const uint32_t old_w = stable_w, old_h = stable_h;
        stable_w = w;
        stable_h = h;
        target_stable = true;
        if (log)
          log(L"render_target: stable size=%ux%u frames=%d elapsed_ms=%llu",
              w, h, candidate_frames,
              (unsigned long long)(now - candidate_first_ms));
        (void)old_w; (void)old_h;
      }
    }
    else
    {
      if (candidate_w != 0 || candidate_h != 0)
      {
        if (log)
          log(L"render_target: unstable/reset old=%ux%u new=%ux%u",
              candidate_w, candidate_h, w, h);
      }
      else
      {
        if (log)
          log(L"render_target: candidate size=%ux%u", w, h);
      }
      candidate_w          = w;
      candidate_h          = h;
      candidate_frames     = 1;
      candidate_first_ms   = GetTickCount64();
      target_stable        = false;
      stable_frames_seen   = 0;
      draw_allowed         = false;
      draw_allowed_logged  = false;
    }

    // OBS-safe: require kObsStableFrames after target is stable
    if (target_stable && obs.detected)
    {
      if (!draw_allowed)
      {
        ++stable_frames_seen;
        if (stable_frames_seen >= kObsStableFrames)
        {
          draw_allowed = true;
          if (!draw_allowed_logged)
          {
            draw_allowed_logged = true;
            if (log)
              log(L"obs_safe: draw allowed stable_frames=%d", stable_frames_seen);
          }
        }
        else
        {
          if (log && (stable_frames_seen == 1))
            log(L"obs_safe: delaying overlay draw until stable_frames=%d",
                kObsStableFrames);
        }
      }
    }
    else if (!obs.detected)
    {
      draw_allowed = target_stable;
    }

    return draw_allowed;
  }
};

// ---------------------------------------------------------------------------
// PART 6 — SKF1 header validation helpers
// ---------------------------------------------------------------------------
static inline bool IsMeaningfulOverlaySize(uint32_t w, uint32_t h)
{
  return w >= 64u && h >= 64u;
}

static inline bool MappedBufferCanContain(uint32_t w, uint32_t h, uint32_t stride,
                                           size_t data_offset,
                                           size_t mapped_bytes)
{
  if (w == 0 || h == 0 || stride == 0) return false;
  if (data_offset >= mapped_bytes)      return false;
  const uint64_t needed = (uint64_t)data_offset + (uint64_t)stride * (uint64_t)h;
  return needed <= (uint64_t)mapped_bytes;
}

// ---------------------------------------------------------------------------
// PART 11 — Pending overlay-state (command thread → render thread)
// ---------------------------------------------------------------------------
// Set atomically from the command/control thread; consumed on the render thread.
inline std::atomic<int>& PendingOverlayMode()
{
  // -1 = no pending change; 0 = off; 1 = toast; 2 = interactive
  static std::atomic<int> s{-1};
  return s;
}

} // namespace SidecarK_OBSCompat
