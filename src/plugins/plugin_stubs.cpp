/**
 * plugin_stubs.cpp — Minimal stub implementations for symbols defined in
 * game-specific plugin translation units that are excluded from Release
 * (SK_SIDECAR_MINIMAL) builds.
 *
 * All stubs are no-ops or return safe zero/null/false values.  They are
 * compiled into every configuration but only provide definitions when
 * SK_SIDECAR_MINIMAL is set, so debug builds keep using the real ones.
 **/

#include <SpecialK/stdafx.h>

#ifdef SK_SIDECAR_MINIMAL

// ---- ReShade headers (needed for reshade::api::effect_runtime*) ----------
#include <SpecialK/plugin/reshade.h>

// ---- Plugin-manager headers (SK_CC_*, SK_Persona4_*, etc.) ---------------
#include <SpecialK/plugin/plugin_mgr.h>

// ---- D3D11 FP16 sanitize (declared in dxgi_hdr.h) -----------------------
#include <SpecialK/render/dxgi/dxgi_hdr.h>

// ---- WinSock offline (declared in utility.h) ----------------------------
#include <SpecialK/utility.h>


// ==========================================================================
//  ReShade stubs
// ==========================================================================

HMODULE __stdcall SK_ReShade_GetDLL (void) { return nullptr; }

void SK_ReShade_LoadIfPresent (void) {}

bool SK_ReShade_IsLocalDLLPresent (void) { return false; }

BOOL SK_ReShade_HasRenoDX (void) { return FALSE; }

bool SK_ReShadeAddOn_Init (HMODULE) { return false; }

void SK_ReShadeAddOn_ActivateOverlay (bool) {}

bool SK_ReShadeAddOn_IsOverlayActive (void) { return false; }

void SK_ReShadeAddOn_ToggleOverlay (void) {}

bool SK_ReShadeAddOn_RenderEffectsD3D11Ex (IDXGISwapChain1*, ID3D11RenderTargetView*, ID3D11RenderTargetView*)
{
  return false;
}

UINT64 SK_ReShadeAddOn_RenderEffectsD3D12 (IDXGISwapChain1*, ID3D12Resource*, ID3D12Fence*,
                                            D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_CPU_DESCRIPTOR_HANDLE)
{
  return 0;
}

reshade::api::effect_runtime*
SK_ReShadeAddOn_GetRuntimeForSwapChain (IDXGISwapChain*) { return nullptr; }

reshade::api::effect_runtime*
SK_ReShadeAddOn_CreateEffectRuntime_D3D11 (ID3D11Device*, ID3D11DeviceContext*, IDXGISwapChain*)
{
  return nullptr;
}

reshade::api::effect_runtime*
SK_ReShadeAddOn_CreateEffectRuntime_D3D12 (ID3D12Device*, ID3D12CommandQueue*, IDXGISwapChain*)
{
  return nullptr;
}

void SK_ReShadeAddOn_CleanupRTVs            (reshade::api::effect_runtime*, bool)  {}
void SK_ReShadeAddOn_UpdateAndPresentEffectRuntime (reshade::api::effect_runtime*) {}
void SK_ReShadeAddOn_DestroyEffectRuntime          (reshade::api::effect_runtime*) {}
void SK_ReShadeAddOn_CleanupConfigAndLogs   (void) {}

// Defined in reshade.cpp normally; referenced via extern in d3d11_core.h
bool SK_D3D11_HasReShadeTriggers = false;


// ==========================================================================
//  Unity stubs  (defined in unity.cpp, referenced via extern)
// ==========================================================================

bool  SK_Unity_PaceGameThread        = false;
void* SK_Unity_GetFrameStatsWaitEvent = nullptr;
void  SK_Unity_InitPlugin (void) {}


// ==========================================================================
//  ChronoCross / D3D11 FP16 stubs  (defined in chrono_cross.cpp /
//  unclassified.cpp, referenced via plugin_mgr.h / dxgi_hdr.h)
// ==========================================================================

float               __SK_CC_ResMultiplier = 0.0f;
ID3D11SamplerState* SK_CC_NearestSampler  = nullptr;

void SK_CC_DrawHandler (ID3D11DeviceContext*, uint32_t, uint32_t) {}

bool SK_D3D11_SanitizeFP16RenderTargets (ID3D11DeviceContext*, UINT) { return false; }


// ==========================================================================
//  Persona 4 stubs  (defined in persona4.cpp)
// ==========================================================================

void SK_Persona4_InitPlugin  (void) {}
void SK_Persona4_DrawHandler (ID3D11DeviceContext*, uint32_t, uint32_t) {}


// ==========================================================================
//  Unclassified / GalGun stub  (defined in unclassified.cpp)
// ==========================================================================

bool SK_GalGun_PlugInCfg (void) { return false; }


// ==========================================================================
//  Launcher stubs  (defined in armoredcore6.cpp / elden_ring.cpp /
//  lords_of_the_fallen2.cpp)
// ==========================================================================

void SK_SEH_LaunchArmoredCoreVI     (void)              {}
void SK_SEH_LaunchEldenRing         (const wchar_t*)    {}
void SK_SEH_LaunchLordsOfTheFallen2 (void)              {}


// ==========================================================================
//  WinSock stub  (defined in sekiro.cpp)
// ==========================================================================

void SK_WinSock_GoOffline (void) {}


// ==========================================================================
//  ImPlot demo stub  (defined in implot_demo.cpp)
// ==========================================================================

namespace ImPlot {
  void ShowDemoWindow (bool*) {}
}


// ==========================================================================
//  x64-only plugin stubs  (symbols only declared under #ifdef _WIN64 in
//  plugin_mgr.h, or referenced via inline extern from scheduler.cpp /
//  debug_utils.cpp / widget.cpp)
// ==========================================================================

#ifdef _WIN64

// -- Dark Souls 3  (defined in dark_souls3.cpp) ----------------------------
void     SK_DS3_InitPlugin      (void)                    {}
bool WINAPI SK_DS3_ShutdownPlugin (const wchar_t*)        { return false; }
HRESULT __stdcall SK_DS3_PresentFirstFrame (IUnknown*, UINT, UINT) { return E_NOTIMPL; }

// -- Tales of Vesperia Fix  (defined in tales_of_vesperia.cpp) -------------
void SK_TVFix_InitPlugin       (void)                       {}
bool SK_TVFix_SharpenShadows   (void)                       { return false; }
void SK_TVFix_CreateTexture2D  (D3D11_TEXTURE2D_DESC*)      {}
bool SK_TVFix_NoRenderSleep    (void)                       { return false; }
bool SK_TVFix_ActiveAntiStutter(void)                       { return false; }

// -- Yakuza 0  (defined in yakuza0.cpp) ------------------------------------
void SK_Yakuza0_PlugInInit (void) {}
bool SK_Yakuza0_PlugInCfg  (void) { return false; }

bool __SK_Yakuza_TrackRTVs   = false;
bool __SK_Y0_SafetyLeak       = false;
bool __SK_Y0_FixAniso         = false;
bool __SK_Y0_ClampLODBias     = false;
int  __SK_Y0_ForceAnisoLevel  = 0;

void SK_YS0_TriggerHudFreeScreenshot (void) {}

// -- Monster Hunter World  (defined in monster_hunter_world.cpp) -----------
void SK_MHW_PlugInInit    (void) {}
void SK_MHW_PlugIn_Shutdown(void) {}

bool __SK_MHW_JobParity         = false;
bool __SK_MHW_JobParityPhysical = false;
bool __SK_MHW_KillAntiDebug     = false;

// -- Shenmue  (defined in unclassified.cpp) --------------------------------
volatile LONG __SK_SHENMUE_FinishedButNotPresented = 0;

bool SK_Shenmue_IsLimiterBypassed   (void)   { return false; }
bool SK_Shenmue_InitLimiterOverride (LPVOID) { return false; }

// -- NieR Replicant v1.5  (defined in nier_replicant_v1.5.cpp) ------------
volatile LONG _SK_NIER_RAD_InputPollingPeriod = 8;

// -- Init/config stubs for remaining x64-only plugins ----------------------
void SK_DGPU_InitPlugin         (void) {}
void SK_IT_InitPlugin           (void) {}
void SK_NNK2_InitPlugin         (void) {}
void SK_YS8_InitPlugin          (void) {}
void SK_ER_InitPlugin           (void) {}
void SK_ELEX2_InitPlugin        (void) {}
void SK_BGS_InitPlugin          (void) {}
void SK_LOTF2_InitPlugin        (void) {}
void SK_OPT_InitPlugin          (void) {}
void SK_ACV_InitPlugin          (void) {}
void SK_ACS_InitPlugin          (void) {}
void SK_ACO_PlugInInit          (void) {}
void SK_DQXI_PlugInInit         (void) {}
void SK_SM_PlugInInit           (void) {}
void SK_NIER_RAD_InitPlugin     (void) {}
void SK_FF7R_InitPlugin         (void) {}
void SK_Sekiro_InitPlugin       (void) {}
void SK_FFXVI_InitPlugin        (void) {}
void SK_FFXV_InitPlugin         (void) {}
void SK_FFXV_SetupThreadPriorities (void) {}
void SK_Metaphor_InitPlugin     (void) {}
void SK_SO2R_InitPlugin         (void) {}
void SK_EnderLilies_InitPlugIn  (void) {}
void SK_SilentHill_f_InitPlugIn (void) {}
void SK_WatchDogs_InitPlugin    (void) {}
void SK_Okami_LoadConfig        (void) {}
void __stdcall SK_HatsuneMiku_BeginFrame (void) {}

bool SK_NIER_RAD_PlugInCfg     (void) { return false; }
bool SK_Okami_PlugInCfg        (void) { return false; }
bool SK_LSBTS_PlugInCfg        (void) { return false; }
bool SK_POE2_PlugInCfg         (void) { return false; }
bool SK_SO4_PlugInCfg          (void) { return false; }
bool SK_ACO_PlugInCfg          (void) { return false; }
bool SK_SM_PlugInCfg           (void) { return false; }
bool SK_FFXV_PlugInCfg         (void) { return false; }
bool SK_FarCry6_PlugInCfg      (void) { return false; }
bool SK_SO2R_PlugInCfg         (void) { return false; }
bool SK_SilentHill_f_PlugInCfg (void) { return false; }

bool SK_SO2R_DrawHandler (ID3D11DeviceContext*, uint32_t, int) { return false; }

// -- FFXV thread-priority management  (defined in unclassified.cpp) --------
//    debug_utils.cpp forward-declares this struct and holds extern refs to
//    the lazy globals; unclassified.cpp provides the definitions.  Provide
//    no-op stubs here so Release x64 links cleanly.
struct SK_FFXV_Thread
{
           HANDLE           hThread  = nullptr;
  volatile LONG             dwPrio   = THREAD_PRIORITY_NORMAL;
  sk::ParameterInt*         prio_cfg = nullptr;

  void setup (HANDLE hNewThread);
};

// Out-of-line definition ensures a globally-visible symbol is emitted.
// An inline body inside the class is not sufficient because debug_utils.cpp
// only has a non-inline declaration and the compiler may elide an
// inline-only definition that is never called within this TU.
void SK_FFXV_Thread::setup (HANDLE /*hNewThread*/) {}

SK_LazyGlobal <SK_FFXV_Thread> sk_ffxv_vsync,
                                sk_ffxv_async_run;

#endif // _WIN64

#endif // SK_SIDECAR_MINIMAL
