// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
/**
 * This file is part of Special K.
 *
 * Special K is free software : you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by The Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Special K is distributed in the hope that it will be useful,
 *
 * But WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Special K.
 *
 *   If not, see <http://www.gnu.org/licenses/>.
 *
**/

//
// SKToast — generic in-frame toast HUD rasterizer.
//
// Header-only, render-only.  Given a bounded UTF-8 text string, produces a
// top-down BGRA bitmap with:
//   - opaque dark rounded-ish rectangle background (alpha=255 throughout)
//   - white text rendered with the OS default GUI font (via GDI)
//
// This module owns NO presentation/composition state — it only fills a
// caller-provided byte buffer.  Backend-specific code (D3D11, OpenGL) is
// responsible for actually getting those pixels onto the backbuffer.
//
// Intentionally does NOT consume the SidecarK control-plane / SKC1 sub-block
// directly; the caller passes raw text bytes already snapshotted from shared
// memory.  This keeps the rasterizer trivially testable and independent of
// the renderer-side seq-lock protocol.
//
// No external assets are required and no caller-specific copy, branding, or
// icon files are baked into SidecarK.
//
#ifndef __SK__TOAST_RENDER_H__
#define __SK__TOAST_RENDER_H__

#include <windows.h>
#include <cstdint>
#include <cstring>
#include <vector>

namespace SK_Toast {

// Standard toast surface size.  Chosen as a bottom-right HUD chip that is
// big enough to read at 1080p and 4K without being intrusive.
inline constexpr uint32_t kSurfaceWidth  = 360u;
inline constexpr uint32_t kSurfaceHeight = 72u;
inline constexpr uint32_t kSurfaceStride = kSurfaceWidth * 4u;  // BGRA

// Padding from the screen edge in pixels.  Bottom-right anchored.
inline constexpr uint32_t kScreenMarginX = 24u;
inline constexpr uint32_t kScreenMarginY = 24u;

// Internal text padding from the toast rect edges.
inline constexpr int      kTextPadX = 16;
inline constexpr int      kTextPadY = 10;

// Background fill (BGRA, premultiplied-alpha not required since we use opaque
// pixels and a raw CopySubresourceRegion / glTexSubImage2D blit).
inline constexpr uint8_t  kBgB = 28u, kBgG = 28u, kBgR = 28u, kBgA = 255u;
// Single-pixel rim for separation from the game frame.
inline constexpr uint8_t  kRimB = 80u, kRimG = 80u, kRimR = 80u, kRimA = 255u;

// Fill the destination BGRA buffer with the toast background (rounded-style
// flat rect with a single-pixel rim).  No GDI required.
inline void FillBackground (uint8_t* dst /* w*h*4 bytes, top-down */)
{
  if (dst == nullptr) return;

  for (uint32_t y = 0; y < kSurfaceHeight; ++y)
  {
    uint8_t* row = dst + (size_t)y * kSurfaceStride;
    const bool rim_row = (y == 0u || y == kSurfaceHeight - 1u);

    for (uint32_t x = 0; x < kSurfaceWidth; ++x)
    {
      const bool rim_col = (x == 0u || x == kSurfaceWidth - 1u);
      const bool rim     = rim_row || rim_col;

      uint8_t* px = row + (size_t)x * 4u;
      if (rim) { px[0] = kRimB; px[1] = kRimG; px[2] = kRimR; px[3] = kRimA; }
      else     { px[0] = kBgB;  px[1] = kBgG;  px[2] = kBgR;  px[3] = kBgA;  }
    }
  }
}

// Rasterize `utf8_text` (bounded; non-NUL-terminated permitted) into the
// destination BGRA buffer.  Background must already be filled (or call
// FillBackground first).
//
// Uses GDI: a top-down DIB section + the OS default GUI font.  No external
// font files are required.  GDI is available unconditionally in any Win32
// process the SidecarK DLL is loaded into.
//
// Returns true if GDI succeeded; false otherwise.  On failure the destination
// buffer is left as-is (background only) — caller should still draw it.
//
// Safe to call from the render thread.  The DIB section and font are
// destroyed before return.  No global GDI state is mutated.
inline bool DrawTextWithGDI (uint8_t*    dst /* w*h*4 bytes, top-down */,
                             const char* utf8_text,
                             uint32_t    text_bytes)
{
  if (dst == nullptr || utf8_text == nullptr || text_bytes == 0u)
    return false;

  // Convert UTF-8 → UTF-16 for DrawTextW.  Bound conservatively: 512 chars
  // input → at most 512 wide chars output.  Be permissive: if the strict
  // UTF-8 conversion rejects the input, fall back to the loose conversion so
  // a visible toast still appears.
  wchar_t wbuf [600] = { };
  int wlen =
    MultiByteToWideChar (CP_UTF8, MB_ERR_INVALID_CHARS,
                         utf8_text, (int)text_bytes,
                         wbuf, (int)(sizeof (wbuf) / sizeof (wbuf [0]) - 1));
  if (wlen <= 0)
  {
    wlen =
      MultiByteToWideChar (CP_UTF8, 0,
                           utf8_text, (int)text_bytes,
                           wbuf, (int)(sizeof (wbuf) / sizeof (wbuf [0]) - 1));
  }
  if (wlen <= 0)
    return false;
  wbuf [wlen] = L'\0';

  HDC screenDC = GetDC (nullptr);
  if (screenDC == nullptr)
    return false;

  HDC memDC = CreateCompatibleDC (screenDC);
  ReleaseDC (nullptr, screenDC);
  if (memDC == nullptr)
    return false;

  BITMAPINFO bmi = { };
  bmi.bmiHeader.biSize        = sizeof (BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth       = (LONG)kSurfaceWidth;
  // Negative height: top-down DIB so row 0 is the top — matches our caller's
  // dst layout, BGRA8 D3D11 textures, and our flipped GL UVs.
  bmi.bmiHeader.biHeight      = -(LONG)kSurfaceHeight;
  bmi.bmiHeader.biPlanes      = 1;
  bmi.bmiHeader.biBitCount    = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  void*  dib_bits = nullptr;
  HBITMAP dib = CreateDIBSection (memDC, &bmi, DIB_RGB_COLORS,
                                  &dib_bits, nullptr, 0);
  if (dib == nullptr || dib_bits == nullptr)
  {
    if (dib != nullptr) DeleteObject (dib);
    DeleteDC (memDC);
    return false;
  }

  // Seed the DIB with the caller's pre-filled background so GDI text overlays
  // it rather than appearing on a black field.
  memcpy (dib_bits, dst, (size_t)kSurfaceStride * kSurfaceHeight);

  HGDIOBJ old_bmp = SelectObject (memDC, dib);

  // Pick a font sized to comfortably fill the toast height.  Negative lfHeight
  // is "character cell" pixels.  Calling GetStockObject is robust across any
  // Windows locale.
  LOGFONTW lf = { };
  lf.lfHeight  = -18;
  lf.lfWeight  = FW_SEMIBOLD;
  lf.lfQuality = CLEARTYPE_QUALITY;
  lf.lfCharSet = DEFAULT_CHARSET;
  // Don't hardcode a face name — let GDI pick the system UI font.
  // lf.lfFaceName left empty.
  HFONT font = CreateFontIndirectW (&lf);
  HGDIOBJ old_font = nullptr;
  if (font != nullptr)
    old_font = SelectObject (memDC, font);

  SetBkMode    (memDC, TRANSPARENT);
  SetTextColor (memDC, RGB (240, 240, 240));

  RECT rc = { kTextPadX,
              kTextPadY,
              (LONG)kSurfaceWidth  - kTextPadX,
              (LONG)kSurfaceHeight - kTextPadY };
  DrawTextW (memDC, wbuf, wlen, &rc,
             DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);

  GdiFlush ();

  // Post-process: GDI writes BGR0 (alpha byte left zeroed by ClearType).
  // Force alpha to 255 across the whole buffer so the blit is fully opaque.
  uint8_t* src = static_cast<uint8_t*> (dib_bits);
  const size_t pixels = (size_t)kSurfaceWidth * kSurfaceHeight;
  for (size_t i = 0; i < pixels; ++i)
    src [i * 4 + 3] = 255u;

  // Copy back into caller's buffer.
  memcpy (dst, dib_bits, (size_t)kSurfaceStride * kSurfaceHeight);

  if (old_font != nullptr) SelectObject (memDC, old_font);
  SelectObject (memDC, old_bmp);
  if (font != nullptr) DeleteObject (font);
  DeleteObject (dib);
  DeleteDC     (memDC);

  return true;
}

// Convenience: produce a full BGRA toast bitmap into `out` (resized to fit).
// Returns true on success.  If GDI fails for any reason, returns true with a
// background-only bitmap so a visible (text-less) toast still appears.
inline bool Rasterize (const char* utf8_text, uint32_t text_bytes,
                      std::vector<uint8_t>& out /* will be sized to w*h*4 */)
{
  const size_t bytes = (size_t)kSurfaceStride * kSurfaceHeight;
  if (out.size () != bytes)
    out.resize (bytes);

  FillBackground (out.data ());
  // Background-only is still acceptable per the spec ("text-only visible
  // toast is better than diagnostic-only").  Treat GDI failure as non-fatal.
  (void)DrawTextWithGDI (out.data (), utf8_text, text_bytes);
  return true;
}

// Format a 32-bit BGRA pixel for D3D11 R8G8B8A8_UNORM by swapping R↔B in
// place.  The default Rasterize() output is BGRA which matches
// DXGI_FORMAT_B8G8R8A8_UNORM directly; this helper supports the R8G8B8A8
// backbuffer format.
inline void SwapToRGBA (uint8_t* buf /* w*h*4 bytes */)
{
  if (buf == nullptr) return;
  const size_t pixels = (size_t)kSurfaceWidth * kSurfaceHeight;
  for (size_t i = 0; i < pixels; ++i)
  {
    const uint8_t b = buf [i * 4 + 0];
    const uint8_t r = buf [i * 4 + 2];
    buf [i * 4 + 0] = r;
    buf [i * 4 + 2] = b;
  }
}

// Compute the destination rectangle (top-left x/y in pixels) of the toast on
// a backbuffer of `bb_width`×`bb_height`.  Bottom-right anchored.  Returns
// false when the backbuffer is too small to render the toast.
inline bool ComputeDestRect (uint32_t  bb_width,
                             uint32_t  bb_height,
                             uint32_t& out_x,
                             uint32_t& out_y)
{
  if (bb_width  < kSurfaceWidth  + kScreenMarginX * 2u) return false;
  if (bb_height < kSurfaceHeight + kScreenMarginY * 2u) return false;

  out_x = bb_width  - kSurfaceWidth  - kScreenMarginX;
  out_y = bb_height - kSurfaceHeight - kScreenMarginY;
  return true;
}

} // namespace SK_Toast

#endif // __SK__TOAST_RENDER_H__
