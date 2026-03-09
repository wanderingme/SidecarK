/**
 * plugin_stubs.cpp — Minimal stub implementations for symbols defined in
 * game-specific plugin translation units that are excluded from Release
 * (SK_SIDECAR_MINIMAL) builds.
 *
 * All stubs are no-ops or return safe zero/null/false values.  They are
 * compiled into every configuration but only provide definitions when
 * SK_SIDECAR_MINIMAL is set, so debug builds keep using the real ones.
 **/

#ifdef SK_SIDECAR_MINIMAL

#include <SpecialK/stdafx.h>

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

#endif // SK_SIDECAR_MINIMAL
