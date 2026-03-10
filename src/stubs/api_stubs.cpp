/**
 * api_stubs.cpp — Stub implementations for symbols defined in API hooking
 * translation units that are excluded from Release (SK_SIDECAR_MINIMAL) builds.
 *
 * These functions are exported by SpecialK.def but the source files that
 * normally define them (d3d9.cpp, d3d8.cpp, ddraw.cpp, dinput8.cpp,
 * xinput_core.cpp, hid.cpp, gpu_monitor.cpp, steam_api.cpp, version.cpp,
 * network.cpp, EULA.cpp) are excluded from SK_SIDECAR_MINIMAL builds.
 *
 * All stubs are no-ops or return safe zero/null/false/E_NOTIMPL values.
 **/

#include <SpecialK/stdafx.h>

#ifdef SK_SIDECAR_MINIMAL

// ---- D3D9 backend headers (IDirect3DDevice9*, D3DPRESENT_PARAMETERS, etc.)
#include <SpecialK/render/d3d9/d3d9_backend.h>

// ---- ddraw.h for DirectDraw types (LPDIRECTDRAW, LPDDENUMCALLBACK, etc.)
#include <ddraw.h>

// ---- GPU monitor header
#include <SpecialK/performance/gpu_monitor.h>

// ---- DInput8 types (SK_DI8_Keyboard, SK_DI8_Mouse)
#include <SpecialK/input/dinput8_backend.h>

// ---- XInput types (XINPUT_STATE)
#include <SpecialK/input/xinput.h>

// ---- HID types (HIDP_REPORT_TYPE, PHIDP_DATA, PHIDP_PREPARSED_DATA, SK_HID_PlayStationDevice)
#include <SpecialK/input/input.h>

// ---- Render backend (SK_RenderBackend_V2, vk_reflex_s, latency_monitor_s)
#include <SpecialK/render/backend.h>

// ---- D3DKMT types
#include <SpecialK/render/d3dkmt/d3dkmt.h>

// ---- CPU diagnostics (SK_FPU_ControlWord)
#include <SpecialK/diagnostics/cpu.h>

// ---- Vulkan headers (SK_Reflex_SetVulkanSwapchain, SK_VK_SetLatencyMarker)
#include <vulkan/vulkan.h>

// ---- Widget registry (SK_ImGui_WidgetRegistry, SK_Widget, SK_Stat_DataHistory)
#include <SpecialK/widgets/widget.h>


// ==========================================================================
//  Direct3D 9 factory / passthrough stubs
// ==========================================================================

IDirect3D9* STDMETHODCALLTYPE Direct3DCreate9   (UINT)                          { return nullptr; }
HRESULT     STDMETHODCALLTYPE Direct3DCreate9Ex (UINT, IDirect3D9Ex** ppD3D)
{
  if (ppD3D) *ppD3D = nullptr;
  return E_NOTIMPL;
}

// Win32-only (d3d8.cpp is excluded for x64 builds too via the project, but
// the DEF exports it unconditionally, so stub it for all arches here).
IUnknown*   STDMETHODCALLTYPE Direct3DCreate8   (UINT)                          { return nullptr; }

void*  __stdcall Direct3DShaderValidatorCreate  (void) { return nullptr; }
void*  __stdcall Direct3DShaderValidatorCreate9 (void) { return nullptr; }
void*  __stdcall DebugSetMute                   (void) { return nullptr; }
int    __stdcall Direct3D9EnableMaximizedWindowedModeShim (BOOL) { return 0; }


// ==========================================================================
//  D3DPERF stubs  (D3D performance annotations -- all no-ops)
// ==========================================================================

int   WINAPI D3DPERF_BeginEvent       (D3DCOLOR, LPCWSTR)         { return 0; }
int   WINAPI D3DPERF_EndEvent         (void)                      { return 0; }
DWORD WINAPI D3DPERF_GetStatus        (void)                      { return 0; }
BOOL  WINAPI D3DPERF_QueryRepeatFrame (void)                      { return FALSE; }
void  WINAPI D3DPERF_SetMarker        (D3DCOLOR, LPCWSTR)         { }
void  WINAPI D3DPERF_SetOptions       (DWORD)                     { }
void  WINAPI D3DPERF_SetRegion        (D3DCOLOR, LPCWSTR)         { }


// ==========================================================================
//  D3D9 device-method override stubs
// ==========================================================================

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9BeginScene_Override (IDirect3DDevice9*)                                              { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9CreateRenderTarget_Override (IDirect3DDevice9*, UINT, UINT, D3DFORMAT,
                                  D3DMULTISAMPLE_TYPE, DWORD, BOOL,
                                  IDirect3DSurface9**, HANDLE*)                         { return E_NOTIMPL; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9CreateDepthStencilSurface_Override (IDirect3DDevice9*, UINT, UINT, D3DFORMAT,
                                         D3DMULTISAMPLE_TYPE, DWORD, BOOL,
                                         IDirect3DSurface9**, HANDLE*)                  { return E_NOTIMPL; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9TestCooperativeLevel_Override (IDirect3DDevice9*)                                   { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetSamplerState_Override (IDirect3DDevice9*, DWORD, D3DSAMPLERSTATETYPE, DWORD)     { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetVertexShaderConstantF_Override (IDirect3DDevice9*, UINT, CONST float*, UINT)     { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetVertexShader_Override (IDirect3DDevice9*, IDirect3DVertexShader9*)               { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetPixelShaderConstantF_Override (IDirect3DDevice9*, UINT, CONST float*, UINT)      { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetPixelShader_Override (IDirect3DDevice9*, IDirect3DPixelShader9*)                 { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetScissorRect_Override (IDirect3DDevice9*, CONST RECT*)                            { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetRenderTarget_Override (IDirect3DDevice9*, DWORD, IDirect3DSurface9*)             { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetDepthStencilSurface_Override (IDirect3DDevice9*, IDirect3DSurface9*)             { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9CreateTexture_Override (IDirect3DDevice9*, UINT, UINT, UINT, DWORD,
                             D3DFORMAT, D3DPOOL, IDirect3DTexture9**, HANDLE*)          { return E_NOTIMPL; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9UpdateTexture_Override (IDirect3DDevice9*, IDirect3DBaseTexture9*,
                             IDirect3DBaseTexture9*)                                     { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetViewport_Override (IDirect3DDevice9*, CONST D3DVIEWPORT9*)                       { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9DrawIndexedPrimitive_Override (IDirect3DDevice9*, D3DPRIMITIVETYPE,
                                   INT, UINT, UINT, UINT, UINT)                         { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9EndScene_Override (IDirect3DDevice9*)                                               { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetTexture_Override (IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9*)              { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9DrawPrimitive_Override (IDirect3DDevice9*, D3DPRIMITIVETYPE, UINT, UINT)            { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9StretchRect_Override (IDirect3DDevice9*, IDirect3DSurface9*, CONST RECT*,
                           IDirect3DSurface9*, CONST RECT*, D3DTEXTUREFILTERTYPE)       { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9Reset_Override (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)                          { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetRenderState_Override (IDirect3DDevice9*, D3DRENDERSTATETYPE, DWORD)             { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9DrawPrimitiveUP_Override (IDirect3DDevice9*, D3DPRIMITIVETYPE, UINT,
                               CONST void*, UINT)                                       { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9DrawIndexedPrimitiveUP_Override (IDirect3DDevice9*, D3DPRIMITIVETYPE,
                                     UINT, UINT, UINT, CONST void*,
                                     D3DFORMAT, CONST void*, UINT)                     { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetCursorPosition_Override (IDirect3DDevice9*, INT, INT, DWORD)                    { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9CreateVertexBuffer_Override (IDirect3DDevice9*, UINT, DWORD, DWORD,
                                  D3DPOOL, IDirect3DVertexBuffer9**, HANDLE*)           { return E_NOTIMPL; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetStreamSource_Override (IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*,
                               UINT, UINT)                                              { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetStreamSourceFreq_Override (IDirect3DDevice9*, UINT, UINT)                       { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetFVF_Override (IDirect3DDevice9*, DWORD)                                         { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9SetVertexDeclaration_Override (IDirect3DDevice9*, IDirect3DVertexDeclaration9*)    { return S_OK; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
D3D9CreateVertexDeclaration_Override (IDirect3DDevice9*, CONST D3DVERTEXELEMENT9*,
                                       IDirect3DVertexDeclaration9**)                  { return E_NOTIMPL; }

__declspec (noinline) D3DPRESENT_PARAMETERS* WINAPI
SK_SetPresentParamsD3D9 (IDirect3DDevice9*, D3DPRESENT_PARAMETERS* pparams)            { return pparams; }


// ==========================================================================
//  DirectDraw stubs  (from ddraw.cpp)
// ==========================================================================

__declspec (noinline) HRESULT WINAPI
DirectDrawCreate (_In_  GUID         FAR *,
                  _Out_ LPDIRECTDRAW FAR *lplpDD,
                  _In_  IUnknown     FAR *)
{
  if (lplpDD) *lplpDD = nullptr;
  return E_NOTIMPL;
}

__declspec (noinline) HRESULT WINAPI
DirectDrawCreateEx (_In_  GUID         FAR *,
                    _Out_ LPDIRECTDRAW FAR *lplpDD,
                    _In_  REFIID,
                    _In_  IUnknown     FAR *)
{
  if (lplpDD) *lplpDD = nullptr;
  return E_NOTIMPL;
}

__declspec (noinline) HRESULT WINAPI
DirectDrawEnumerateA (_In_ LPDDENUMCALLBACK, _In_ LPVOID) { return E_NOTIMPL; }

__declspec (noinline) HRESULT WINAPI
DirectDrawEnumerateW (_In_ LPDDENUMCALLBACK, _In_ LPVOID) { return E_NOTIMPL; }

__declspec (noinline) HRESULT WINAPI
DirectDrawEnumerateExA (_In_ LPDDENUMCALLBACKEX, _In_ LPVOID, _In_ DWORD) { return E_NOTIMPL; }

__declspec (noinline) HRESULT WINAPI
DirectDrawEnumerateExW (_In_ LPDDENUMCALLBACKEX, _In_ LPVOID, _In_ DWORD) { return E_NOTIMPL; }

__declspec (noinline) HRESULT __stdcall
DllCanUnloadNow_Override (void) { return S_FALSE; }


// ==========================================================================
//  DirectInput 8 stubs  (from dinput8.cpp)
// ==========================================================================

__declspec (noinline) HRESULT WINAPI
DirectInput8Create (HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN) { return E_NOTIMPL; }

__declspec (noinline) HRESULT STDMETHODCALLTYPE
IDirectInputDevice8_GetDeviceState_Detour (LPDIRECTINPUTDEVICE8, DWORD, LPVOID) { return E_NOTIMPL; }

__declspec (noinline) SK_DI8_Keyboard* WINAPI
SK_Input_GetDI8Keyboard (void) { return nullptr; }

__declspec (noinline) SK_DI8_Mouse* WINAPI
SK_Input_GetDI8Mouse (void) { return nullptr; }


// ==========================================================================
//  HID stub  (from hid.cpp)
// ==========================================================================

__declspec (noinline) NTSTATUS __stdcall
HidP_GetData_Detour (_In_    HIDP_REPORT_TYPE     ReportType,
                     _Out_   PHIDP_DATA           DataList,
                     _Inout_ PULONG               DataLength,
                     _In_    PHIDP_PREPARSED_DATA PreparsedData,
                     _In_    PCHAR                Report,
                     _In_    ULONG                ReportLength)
{
  (void)ReportType; (void)DataList; (void)DataLength;
  (void)PreparsedData; (void)Report; (void)ReportLength;
  return (NTSTATUS)0; // HIDP_STATUS_SUCCESS
}


// ==========================================================================
//  XInput stubs  (from xinput_core.cpp)
// ==========================================================================

__declspec (noinline) DWORD WINAPI
XInputGetState1_4_Detour (_In_  DWORD dwUserIndex, _Out_ XINPUT_STATE *pState)
{
  (void)dwUserIndex;
  if (pState) *pState = {};
  return ERROR_DEVICE_NOT_CONNECTED;
}

__declspec (noinline) DWORD WINAPI
XInputGetState1_3_Detour (_In_  DWORD dwUserIndex, _Out_ XINPUT_STATE *pState)
{
  return XInputGetState1_4_Detour (dwUserIndex, pState);
}

__declspec (noinline) DWORD WINAPI
XInputGetState9_1_0_Detour (_In_  DWORD dwUserIndex, _Out_ XINPUT_STATE *pState)
{
  return XInputGetState1_4_Detour (dwUserIndex, pState);
}

__declspec (noinline) bool WINAPI
SK_XInput_PollController (INT, XINPUT_STATE*) { return false; }


// ==========================================================================
//  GPU monitor stubs  (from gpu_monitor.cpp)
// ==========================================================================

uint32_t __stdcall SK_GPU_GetClockRateInkHz    (int) { return 0; }
uint32_t __stdcall SK_GPU_GetMemClockRateInkHz (int) { return 0; }
uint64_t __stdcall SK_GPU_GetMemoryBandwidth   (int) { return 0; }
float    __stdcall SK_GPU_GetMemoryLoad        (int) { return 0.0f; }
float    __stdcall SK_GPU_GetGPULoad           (int) { return 0.0f; }
float    __stdcall SK_GPU_GetTempInC           (int) { return 0.0f; }
uint32_t __stdcall SK_GPU_GetFanSpeedRPM       (int) { return 0; }
uint64_t __stdcall SK_GPU_GetVRAMUsed          (int) { return 0; }
uint64_t __stdcall SK_GPU_GetVRAMShared        (int) { return 0; }
uint64_t __stdcall SK_GPU_GetVRAMBudget        (int) { return 0; }


// ==========================================================================
//  Steam API stubs  (from steam_api.cpp)
// ==========================================================================

bool     __stdcall SK_SteamAPI_TakeScreenshot           (void)                               { return false; }
void     __stdcall SK_SteamAPI_SetOverlayState          (bool)                               { }
void     WINAPI    SK_SteamAPI_WriteScreenshot           (void*, uint32_t, int, int)          { }
void     WINAPI    SK_SteamAPI_AddScreenshotToLibrary    (const char*, const char*, int, int) { }
uint32_t __stdcall SK_Steam_PiratesAhoy2                (void)                               { return 0; }


// ==========================================================================
//  Version / Update stubs  (from version.cpp / network.cpp)
// ==========================================================================

bool    __stdcall SK_FetchVersionInfo1 (const wchar_t*, bool)   { return false; }
bool    __stdcall SK_FetchVersionInfo  (const wchar_t*)         { return false; }
HRESULT __stdcall SK_UpdateSoftware1   (const wchar_t*, bool)   { return S_FALSE; }
HRESULT __stdcall SK_UpdateSoftware    (const wchar_t*)         { return S_FALSE; }


// ==========================================================================
//  ImGui EULA stub  (from EULA.cpp)
// ==========================================================================

void __stdcall SK_ImGui_DrawEULA (LPVOID) { }



// ==========================================================================
//  Function pointer globals — hooks.cpp  (defined in excluded debug_utils.cpp
//  and diagnostics/memory.cpp; declared extern in their respective headers)
// ==========================================================================

GetCommandLineA_pfn    GetCommandLineA_Original    = nullptr;
GetCommandLineW_pfn    GetCommandLineW_Original    = nullptr;
CloseHandle_pfn        CloseHandle_Original        = nullptr;
ExitProcess_pfn        ExitProcess_Original        = nullptr;
ExitThread_pfn         ExitThread_Original         = nullptr;
TerminateProcess_pfn   TerminateProcess_Original   = nullptr;
TerminateThread_pfn    TerminateThread_Original    = nullptr;
NtTerminateProcess_pfn NtTerminateProcess_Original = nullptr;
RtlExitUserThread_pfn  RtlExitUserThread_Original  = nullptr;
OutputDebugStringA_pfn OutputDebugStringA_Original = nullptr;
OutputDebugStringW_pfn OutputDebugStringW_Original = nullptr;
RaiseException_pfn     RaiseException_Original     = nullptr;
SetLastError_pfn       SetLastError_Original       = nullptr;
GetProcAddress_pfn     GetProcAddress_Original     = nullptr;
SHGetKnownFolderPath_pfn SHGetKnownFolderPath_Original = nullptr;
_endthreadex_pfn       _endthreadex_Original       = nullptr;

HeapFree_pfn           HeapFree_Original           = nullptr;
VirtualFree_pfn        VirtualFree_Original        = nullptr;
VirtualAlloc_pfn       VirtualAlloc_Original       = nullptr;
GlobalAlloc_pfn        GlobalAlloc_Original        = nullptr;
GlobalFree_pfn         GlobalFree_Original         = nullptr;
LocalAlloc_pfn         LocalAlloc_Original         = nullptr;
LocalFree_pfn          LocalFree_Original          = nullptr;
RtlAllocateHeap_pfn    RtlAllocateHeap_Original    = nullptr;

BOOL __stdcall
SK_Module_IsProcAddrLocal ( HMODULE                    hModExpected,
                             LPCSTR                     lpProcName,
                            FARPROC                     lpProcAddr,
          PLDR_DATA_TABLE_ENTRY__SK *ppldrEntry )
{
  (void)hModExpected; (void)lpProcName; (void)lpProcAddr; (void)ppldrEntry;
  return FALSE;
}


// ==========================================================================
//  TLS globals  (defined in excluded diagnostics/memory.cpp)
// ==========================================================================

volatile LONG _SK_IgnoreTLSAlloc = 0;
volatile LONG _SK_IgnoreTLSMap   = 0;


// ==========================================================================
//  DInput COM factory stubs  (defined in excluded dinput7/8 backends)
// ==========================================================================

HRESULT WINAPI
CoCreateInstance_DI8 ( _In_  LPUNKNOWN pUnkOuter,
                        _In_  DWORD     dwClsContext,
                        _In_  REFIID    riid,
                        _Out_ LPVOID   *ppv,
                        _In_  LPVOID    pCallerAddr )
{
  (void)pUnkOuter; (void)dwClsContext; (void)riid;
  (void)pCallerAddr;
  if (ppv) *ppv = nullptr;
  return E_NOTIMPL;
}

HRESULT WINAPI
CoCreateInstance_DI7 ( _In_  LPUNKNOWN pUnkOuter,
                        _In_  DWORD     dwClsContext,
                        _In_  REFIID    riid,
                        _Out_ LPVOID   *ppv,
                        _In_  LPVOID    pCallerAddr )
{
  (void)pUnkOuter; (void)dwClsContext; (void)riid;
  (void)pCallerAddr;
  if (ppv) *ppv = nullptr;
  return E_NOTIMPL;
}

HRESULT STDAPICALLTYPE
CoCreateInstanceEx_DI8 ( _In_    REFCLSID     rclsid,
                          _In_    IUnknown     *punkOuter,
                          _In_    DWORD        dwClsCtx,
                          _In_    COSERVERINFO *pServerInfo,
                          _In_    DWORD        dwCount,
                          _Inout_ MULTI_QI     *pResults,
                          _In_    LPVOID        pCallerAddr )
{
  (void)rclsid; (void)punkOuter; (void)dwClsCtx;
  (void)pServerInfo; (void)dwCount; (void)pResults; (void)pCallerAddr;
  return E_NOTIMPL;
}

HRESULT STDAPICALLTYPE
CoCreateInstanceEx_DI7 ( _In_    REFCLSID     rclsid,
                          _In_    IUnknown     *punkOuter,
                          _In_    DWORD        dwClsCtx,
                          _In_    COSERVERINFO *pServerInfo,
                          _In_    DWORD        dwCount,
                          _Inout_ MULTI_QI     *pResults,
                          _In_    LPVOID        pCallerAddr )
{
  (void)rclsid; (void)punkOuter; (void)dwClsCtx;
  (void)pServerInfo; (void)dwCount; (void)pResults; (void)pCallerAddr;
  return E_NOTIMPL;
}


// ==========================================================================
//  Window / XInput / HID stubs  (window.cpp, excluded xinput/hid backends)
// ==========================================================================

bool SK_XInput_Enable (int) { return false; }
void SK_HID_FlushPlayStationForceFeedback (void) { }


// ==========================================================================
//  DKMT globals  (defined in excluded render/d3dkmt/d3dkmt.cpp)
// ==========================================================================

extern "C" FARPROC D3DKMTOpenAdapterFromGdiDisplayName    = nullptr;
extern "C" FARPROC D3DKMTGetMultiPlaneOverlayCaps         = nullptr;
extern "C" FARPROC D3DKMTGetScanLine                      = nullptr;

extern "C" FARPROC D3DKMTCloseAdapter                     = nullptr;
extern "C" FARPROC D3DKMTCreateAllocation                 = nullptr;
extern "C" FARPROC D3DKMTCreateContext                    = nullptr;
extern "C" FARPROC D3DKMTCreateDevice                     = nullptr;
extern "C" FARPROC D3DKMTCreateSynchronizationObject      = nullptr;
extern "C" FARPROC D3DKMTDestroyAllocation                = nullptr;
extern "C" FARPROC D3DKMTDestroyContext                   = nullptr;
extern "C" FARPROC D3DKMTDestroyDevice                    = nullptr;
extern "C" FARPROC D3DKMTDestroySynchronizationObject     = nullptr;
extern "C" FARPROC D3DKMTEscape                           = nullptr;
extern "C" FARPROC D3DKMTGetContextSchedulingPriority     = nullptr;
extern "C" FARPROC D3DKMTGetDeviceState                   = nullptr;
extern "C" FARPROC D3DKMTGetDisplayModeList               = nullptr;
extern "C" FARPROC D3DKMTGetMultisampleMethodList         = nullptr;
extern "C" FARPROC D3DKMTGetRuntimeData                   = nullptr;
extern "C" FARPROC D3DKMTGetSharedPrimaryHandle           = nullptr;
extern "C" FARPROC D3DKMTLock                             = nullptr;
extern "C" FARPROC D3DKMTOpenAdapterFromHdc               = nullptr;
extern "C" FARPROC D3DKMTOpenAdapterFromLuid              = nullptr;
extern "C" FARPROC D3DKMTOpenResource                     = nullptr;
extern "C" FARPROC D3DKMTPresent                          = nullptr;
extern "C" FARPROC D3DKMTQueryAdapterInfo                 = nullptr;
extern "C" FARPROC D3DKMTQueryAllocationResidency         = nullptr;
extern "C" FARPROC D3DKMTQueryResourceInfo                = nullptr;
extern "C" FARPROC D3DKMTRender                           = nullptr;
extern "C" FARPROC D3DKMTSetAllocationPriority            = nullptr;
extern "C" FARPROC D3DKMTSetContextSchedulingPriority     = nullptr;
extern "C" FARPROC D3DKMTSetDisplayMode                   = nullptr;
extern "C" FARPROC D3DKMTSetDisplayPrivateDriverFormat    = nullptr;
extern "C" FARPROC D3DKMTSetGammaRamp                     = nullptr;
extern "C" FARPROC D3DKMTSetVidPnSourceOwner              = nullptr;
extern "C" FARPROC D3DKMTSignalSynchronizationObject      = nullptr;
extern "C" FARPROC D3DKMTUnlock                           = nullptr;
extern "C" FARPROC D3DKMTWaitForSynchronizationObject     = nullptr;
extern "C" FARPROC D3DKMTWaitForVerticalBlankEvent        = nullptr;


// ==========================================================================
//  D3DKMT wrappers  (defined in excluded d3dkmt.cpp)
// ==========================================================================

HRESULT SK_D3DKMT_CloseAdapter     (D3DKMT_CLOSEADAPTER     *p) { (void)p; return E_NOTIMPL; }
HRESULT SK_D3DKMT_QueryAdapterInfo (D3DKMT_QUERYADAPTERINFO *p) { (void)p; return E_NOTIMPL; }


// ==========================================================================
//  Render backend class method stubs  (defined in excluded reflex.cpp / d3d9 texmgr)
// ==========================================================================

ULONG64 SK_RenderBackend_V2::vk_reflex_s::sleep           (void)       { return 0; }
bool    SK_RenderBackend_V2::vk_reflex_s::isPacingEligible (void) const { return false; }
bool    SK_RenderBackend_V2::vk_reflex_s::needsFallbackSleep (void) const { return false; }
void    SK_RenderBackend_V2::driverSleepNV (int) const { }

SK_RenderBackend_V2::latency_monitor_s SK_RenderBackend_V2::latency;

void SK_RenderBackend_V2::latency_monitor_s::reset (void) { }
void SK_RenderBackend_V2::latency_monitor_s::submitQueuedFrame (IDXGISwapChain1*) { }

// SK::D3D9::TextureManager::Init/Hook — guarded at call site in render_backend.cpp


// ==========================================================================
//  Render stubs  (excluded reflex.cpp, d3d9.cpp, ddraw.cpp, d3d11_hdr, etc.)
// ==========================================================================

UINT SK_Reflex_CalculateSleepMinIntervalForVulkan (bool) { return 0; }

void SK_Reflex_SetVulkanSwapchain (VkDevice, VkSwapchainKHR) { }

void SK_VK_SetLatencyMarker (VkSetLatencyMarkerInfoNV&, VkLatencyMarkerNV) { }

void __stdcall SK_HookD3D9  (void) { }
void __stdcall SK_HookD3D8  (void) { }
void __stdcall SK_HookDDraw (void) { }

void SK_D3D9_QuickHook         (void) { }
void SK_D3D9_InitShaderModTools (void) { }

void SK_HDR_UpdateMaxLuminanceForActiveDisplay (bool) { }
void SK_HDR_DisableOverridesForGame            (void) { }


// ==========================================================================
//  HDR globals  (defined in excluded widgets/hdr.cpp)
// ==========================================================================

bool  __SK_HDR_10BitSwap        = false;
bool  __SK_HDR_16BitSwap        = false;
bool  __SK_HDR_TonemapOverbright = true;

float __SK_HDR_Luma             =  80.0f;
float __SK_HDR_Exp              =   1.0f;
float __SK_HDR_Saturation       =   1.0f;
float __SK_HDR_Gamut            =  0.01f;
float __SK_HDR_HorizCoverage    = 100.0f;
float __SK_HDR_VertCoverage     = 100.0f;
float __SK_HDR_ColorBoost       =   1.0f;
float __SK_HDR_PQBoost0         =   0.0f;
float __SK_HDR_PQBoost1         =   0.0f;
float __SK_HDR_PQBoost2         =   0.0f;
float __SK_HDR_PQBoost3         =   0.0f;


// ==========================================================================
//  Streamline  (defined in excluded streamline.cpp)
// ==========================================================================

sl::Result SK_slGetNativeInterface (void*, void**) { return sl::Result::eErrorFeatureNotSupported; }


// ==========================================================================
//  Screenshot  (defined in excluded screenshot.cpp)
// ==========================================================================

#include <SpecialK/render/screenshot.h>
void SK_Screenshot_ProcessQueue (SK_ScreenshotStage stage, const SK_RenderBackend_V2& rb)
{
  (void)stage; (void)rb;
}


// ==========================================================================
//  WMI / perf monitor globals  (defined in excluded io_monitor.cpp)
// ==========================================================================

SK_LazyGlobal <cpu_perf_t>      SK_WMI_CPUStats;
SK_LazyGlobal <disk_perf_t>     SK_WMI_DiskStats;
SK_LazyGlobal <pagefile_perf_t> SK_WMI_PagefileStats;


// ==========================================================================
//  Framerate stubs  (excluded widgets/cpu_widget.cpp, frame_pacing.cpp)
// ==========================================================================

// SK_ImGui_FrameHistory is defined in framerate.cpp (always compiled).
// SK_ImGui_Frames storage is provided there under #ifdef SK_SIDECAR_MINIMAL.

bool    __SK_DoubleUpOnReflex         = false;
bool    SK_CPU_IsZen                  (void)    { return false; }
float   SK_Framerate_GetPercentileByIdx (int)   { return 0.0f; }

long (__stdcall* Direct3DCreate9Ex_Import)(unsigned int, IDirect3D9Ex**) = nullptr;

LONG __stdcall SK_DWM_GetCompositionTimingInfo (DWM_TIMING_INFO *p)
{
  if (p) *p = {};
  return E_FAIL;
}

SK_FPU_ControlWord SK_FPU_SetControlWord (UINT mask, SK_FPU_ControlWord *pNew)
{
  (void)mask; (void)pNew;
  return { 0, 0 };
}

SK_FPU_ControlWord SK_FPU_SetPrecision (UINT) { return { 0, 0 }; }
void               SK_FPU_LogPrecision (void) { }

void SK_ImGui_DrawCPUPower       (void) { }
void SK_ImGui_DrawCPUTemperature (void) { }


// ==========================================================================
//  Config stubs  (excluded debug_utils.cpp, steam_api.cpp, widgets/widget.cpp,
//                 diagnostics/file.cpp, etc.)
// ==========================================================================

bool SK_Debug_CheckDebugFlagInPEB (void)             { return false; }
int  SK_Steam_PopupOriginWStrToEnum (const wchar_t*) { return 0; }

iSK_Logger *read_log  = nullptr;
iSK_Logger *write_log = nullptr;

uint64_t SK_Steam_GetAppID_NoAPI (void) { return 0; }

void* SK_Debug_GetImageBaseAddr (void) { return nullptr; }

void __stdcall RtlAcquirePebLock_Detour (void) { }
void __stdcall RtlReleasePebLock_Detour (void) { }

const wchar_t* SK_Steam_PopupOriginToWStr (int) { return L"TopLeft"; }

void __cdecl SteamAPI_ManualDispatch_Init_Detour (void) { }

BOOL SK_ImGui_WidgetRegistry::SaveConfig (void) { return FALSE; }


// ==========================================================================
//  Core stubs  (excluded steam_api.cpp, wasapi.cpp, etw.cpp, network.cpp, etc.)
// ==========================================================================

bool SK_ETW_EndTracing                   (void)              { return false; }
bool SK_File_CanUserWriteToPath          (const wchar_t*)    { return false; }
bool SK_Power_InitEffectiveModeCallbacks (void)              { return false; }
bool SK_Power_StopEffectiveModeCallbacks (void)              { return false; }
bool SK_Steam_LoadOverlayEarly           (void)              { return false; }
bool SK_Steam_TestImports (HINSTANCE *)                      { return false; }
bool __stdcall SK_WASAPI_Init            (void)              { return false; }

SK_Widget* SK_HDR_GetWidget (void) { return nullptr; }

std::wstring SK_Network_MakeEscapeSequencedURL (std::wstring) { return {}; }

int  SK_Steam_KickStart   (const wchar_t*) { return 0; }
long SK_Decompress7zEx    (const wchar_t*, const wchar_t*,
                            int (__stdcall*)(int,int,int)) { return 0; }

DWORD WINAPI SK_MonitorCPU      (LPVOID) { return 0; }
DWORD WINAPI SK_MonitorDisk     (LPVOID) { return 0; }
DWORD WINAPI SK_MonitorPagefile (LPVOID) { return 0; }

void SK::Diagnostics::CrashHandler::Reinstall (void) { }
void SK::Diagnostics::CrashHandler::Shutdown  (void) { }
void SK::SteamAPI::Shutdown                   (void) { }
// SK::Xbox::Init — guarded at call site in core.cpp

void SK_EndGPUPolling                     (void) { }
void SK_File_InitHooks                    (void) { }
void SK_GPU_InitSensorData                (void) { }
void SK_Input_EnumOpenHIDFiles            (void) { }
void SK_Memory_InitHooks                  (void) { }
void SK_NGX_UpdateDLSSGStatus             (void) { }
void SK_Network_InitHooks                 (void) { }
void SK_Platform_PingBackendForNonSteamGame (void) { }
void SK_ScePad_PaceMaker                  (void) { }
void SK_Steam_InitCommandConsoleVariables (void) { }

void __stdcall SK_SetGameMute (bool) { }

const wchar_t* SK_Steam_GetDLLPath (void) { return L""; }

// SymGetSearchPathW (_SymGetSearchPathW@12) and SymSetExtendedOption (_SymSetExtendedOption@8)
// are called via function pointers in core.cpp – provide definitions so the
// decorated (stdcall) symbols resolve at link time.
#include <dbghelp.h>
BOOL IMAGEAPI SymGetSearchPathW (HANDLE, PWSTR, DWORD)           { return FALSE; }
BOOL IMAGEAPI SymSetExtendedOption (IMAGEHLP_EXTENDED_OPTIONS, BOOL) { return FALSE; }


// ==========================================================================
//  HID / PlayStation device stubs (excluded hid.cpp / playstation.cpp)
// ==========================================================================

XINPUT_STATE     hid_to_xi      = {};
volatile ULONG64 hid_to_xi_time = 0;

bool SK_HID_PlayStationDevice::request_input_report (void)          { return false; }
XINPUT_STATE SK_HID_PlayStationDevice::hid_to_xi::getLatestState (void) { return {}; }

void SK_HID_ProcessGamepadButtonBindings (void) { }
void SK_HID_SetupPlayStationControllers  (void) { }
void SK_Input_HookWinMM                  (void) { }
void SK_TriggerHudFreeScreenshot         (void) noexcept { }
void SK_XInput_Refresh                   (UINT) { }


// ==========================================================================
//  Debug / utility stubs  (from debug_utils.cpp / utility.cpp)
// ==========================================================================

BOOL  WINAPI SK_IsDebuggerPresent     (void)              { return IsDebuggerPresent (); }
void  WINAPI SK_OutputDebugStringA    (LPCSTR  msg)       { OutputDebugStringA (msg); }
void  WINAPI SK_OutputDebugStringW    (LPCWSTR msg)       { OutputDebugStringW (msg); }
LPVOID WINAPI SK_AddVectoredExceptionHandler (ULONG first, PVECTORED_EXCEPTION_HANDLER handler)
                                              { return AddVectoredExceptionHandler (first, handler); }
HLOCAL WINAPI SK_LocalAlloc  (UINT uFlags, SIZE_T uBytes) noexcept { return LocalAlloc  (uFlags, uBytes); }
HLOCAL WINAPI SK_LocalFree   (HLOCAL hMem)                noexcept { return LocalFree   (hMem); }

void SK_RaiseException (DWORD code, DWORD flags, DWORD nArgs, const ULONG_PTR* args)
{
  RaiseException (code, flags, nArgs, args);
}

BOOL WINAPI SK_GetSystemCpuSetInformation (
  PSYSTEM_CPU_SET_INFORMATION Info, ULONG BufLen, PULONG RetLen,
  HANDLE Process, ULONG Flags)
{
  return GetSystemCpuSetInformation (Info, BufLen, RetLen, Process, Flags);
}

uint32_t __cdecl safe_crc32c (uint32_t, const void*, size_t) { return 0; }

void SK_Input_PreHookDI8 (void) { }

// ==========================================================================
//  Reflex / backend globals  (from excluded reflex/streamline TUs)
// ==========================================================================

volatile ULONG64 SK_Reflex_LastFrameMarked = 0;

// ==========================================================================
//  Widget registry  (from excluded widget.cpp / control_panel files)
// ==========================================================================

BOOL SK_ImGui_WidgetRegistry::DispatchKeybinds (BOOL, BOOL, BOOL, BYTE) { return FALSE; }

// ==========================================================================
//  Additional HDR globals  (from excluded widgets/hdr.cpp)
// ==========================================================================

bool  __SK_HDR_AdaptiveToneMap  = false;
bool  __SK_HDR_UserForced       = false;
bool  __SK_HDR_Disallow16BitSwap = false;


// ==========================================================================
//  Forward declarations for heavy types (avoids including d3d11_core.h,
//  dxgi_hdr.h, d3d9_device.h, etc.)
// ==========================================================================

struct sk_d3d9_swap_dispatch_s;

struct SK_HDR_RenderTargetManager {
  bool             PromoteTo16Bit  = false;
  volatile LONG64  BytesAllocated  = 0LL;
  volatile ULONG   TargetsUpgraded = 0UL;
  volatile ULONG   CandidatesSeen  = 0UL;
};

struct SK_D3D11_TexCacheResidency_s {
  struct { volatile LONG InVRAM = 0, Shared = 0, PagedOut = 0; } count;
  struct { volatile LONG64 InVRAM = 0, Shared = 0, PagedOut = 0; } size;
};


// ==========================================================================
//  Global variable definitions — control panel, ImGui, screenshot queues
// ==========================================================================

bool reset_frame_history = false;

bool SK_ImGui_Visible              = false;
bool SK_ImGuiEx_Visible            = false;
bool SK_ImGui_WantExit             = false;
bool SK_ImGui_GamepadComboDialogActive = false;

BOOL SK_ImGui_NewInput                       = FALSE;
volatile DWORD SK_ImGui_LastKeyboardProcMessageTime = 0;

int  __SK_HDR_Preset = 0;

namespace SK {
  namespace ControlPanel {
    SK_RenderAPI render_api  = SK_RenderAPI::Reserved;
    uint64_t     current_tick = 0;
    DWORD        current_time = 0;
    font_cfg_s   font         = { };

    namespace D3D11 {
      bool show_shader_mod_dlg = false;
    }
  }
}


// ==========================================================================
//  Screenshot queue globals  (excluded screenshot.cpp)
// ==========================================================================

SK_ScreenshotQueue      enqueued_screenshots = { };
SK_ScreenshotQueue      enqueued_sounds      = { };
SK_ScreenshotTitleQueue enqueued_titles;

SK_ScreenshotQueue::MemoryTotals    SK_ScreenshotQueue::pooled;
SK_Screenshot::framebuffer_s::PinnedBuffer SK_Screenshot::framebuffer_s::root_;


// ==========================================================================
//  HDR render-target manager globals  (excluded d3d11_hdr.cpp)
// ==========================================================================

SK_LazyGlobal <SK_HDR_RenderTargetManager> SK_HDR_RenderTargets_8bpc;
SK_LazyGlobal <SK_HDR_RenderTargetManager> SK_HDR_RenderTargets_10bpc;
SK_LazyGlobal <SK_HDR_RenderTargetManager> SK_HDR_RenderTargets_11bpc;

SK_LazyGlobal <SK_HDR_RenderTargetManager> SK_HDR_UnorderedViews_8bpc;
SK_LazyGlobal <SK_HDR_RenderTargetManager> SK_HDR_UnorderedViews_10bpc;
SK_LazyGlobal <SK_HDR_RenderTargetManager> SK_HDR_UnorderedViews_11bpc;


// ==========================================================================
//  D3D11 texture-cache residency global  (excluded d3d11.cpp / d3d11_tex.cpp)
// ==========================================================================

SK_LazyGlobal <SK_D3D11_TexCacheResidency_s> SK_D3D11_TexCacheResidency;


// ==========================================================================
//  Steam LazyGlobal and function-pointer globals
// ==========================================================================

SK_LazyGlobal <SK_SteamAPIContext> pSteamCtx;

volatile LONGLONG SK_SteamAPI_CallbackRunCount   = 0LL;
volatile LONG     SK_SteamAPI_CallbackRateLimit  = 0L;

extern "C" SteamAPI_UnregisterCallResult_pfn SteamAPI_UnregisterCallResult = nullptr;


// ==========================================================================
//  HID PlayStation controller list  (excluded hid.cpp)
// ==========================================================================

Concurrency::concurrent_vector <SK_HID_PlayStationDevice> SK_HID_PlayStationControllers;


// ==========================================================================
//  DLSS / NGX globals  (excluded ngx.cpp)
// ==========================================================================

bool __SK_ForceDLSSGPacing = false;


// ==========================================================================
//  Volatile screenshot / WinRing0 counters
// ==========================================================================

volatile LONG __SK_ScreenShot_CapturingHUDless = 0L;
volatile LONG __SK_WR0_Init                    = 0L;


// ==========================================================================
//  Render backend static members  (excluded reflex.cpp / d3d11_hdr.cpp)
// ==========================================================================

volatile LONG SK_RenderBackend_V2::flip_skip = 0L;


// ==========================================================================
//  SteamAPI function stubs
// ==========================================================================

bool __stdcall SK_IsSteamOverlayActive (void) { return false; }

uint32_t __stdcall SK_Steam_PiratesAhoy (void) { return 0; }

uint32_t __stdcall
SK_SteamAPI_AddScreenshotToLibraryEx ( const char*, const char*, int, int,
                                       bool ) { return 0; }

int  SK_HookSteamAPI             (void)                     { return 0; }
int  SK_Steam_PreHookCore        (const wchar_t*)           { return 0; }
void SK_SteamAPI_InitManagers    (void)                     { }
void SK_SteamInput_Unfux0r       (void)                     { }
void SK_Steam_CatastropicScreenshotFail (void)              { }
void SK_Steam_ForceInputAppId    (uint64_t)                 { }
void SK_Steam_ProcessWindowActivation (bool)                { }

namespace SK {
  namespace SteamAPI {
    bool         SetWindowFocusState (bool)   { return false; }
    uint64_t     AppID               (void)   { return 0; }
    bool __stdcall TakeScreenshot    ( SK_ScreenshotStage, bool,
                                       std::string )        { return false; }
  }
}


// ==========================================================================
//  Platform / overlay stubs
// ==========================================================================

bool SKC_IsOverlayEnabled         (void)  { return false; }

bool SK_Platform_GetOverlayState  (bool)  { return false; }
bool SK_Platform_SetOverlayState  (bool)  { return false; }
bool SK_Platform_IsOverlayAware   (void)  { return false; }

bool __stdcall SK_Xbox_GetOverlayState (bool) { return false; }


// ==========================================================================
//  D3D backend Startup / Shutdown stubs
// ==========================================================================

namespace SK {
  namespace D3D8  { bool Startup(void) { return false; } bool Shutdown(void) { return false; } }
  namespace D3D9  { bool Startup(void) { return false; } bool Shutdown(void) { return false; }
    std::string __stdcall getPipelineStatsDesc (void) { return { }; }
  }
  namespace DDraw { bool Startup(void) { return false; } bool Shutdown(void) { return false; } }
  namespace DI8   { bool Startup(void) { return false; } bool Shutdown(void) { return false; } }
}


// ==========================================================================
//  D3D9 stubs  (excluded d3d9.cpp)
// ==========================================================================

HRESULT __stdcall SK_D3D9_Present_GrandCentral (sk_d3d9_swap_dispatch_s*) { return S_OK; }
void              SK_D3D9_TriggerReset          (bool)                    { }


// ==========================================================================
//  D3D11 stubs  (excluded d3d11.cpp / d3d11_screenshot.cpp)
// ==========================================================================

bool SK_D3D11_ShouldSkipHUD    (void) { return false; }
bool SK_D3D11_ShowShaderModDlg (void) noexcept { return false; }

bool SK_Screenshot_D3D11_BeginFrame   (void) { return false; }
void SK_Screenshot_D3D11_EndFrame     (void) { }
void SK_Screenshot_D3D11_RestoreHUD   (void) { }
void SK_D3D11_ProcessScreenshotQueueEx (SK_ScreenshotStage, bool, bool) { }


// ==========================================================================
//  Screenshot stubs  (excluded screenshot.cpp / d3d11_screenshot.cpp)
// ==========================================================================

bool SK_Screenshot_IsCapturingHUDless (void) { return false; }
void SK_Screenshot_PlaySound          (void) { }

bool SK_Screenshot_SaveAVIF (DirectX::ScratchImage&, const wchar_t*, uint16_t, uint16_t)
                             { return false; }
bool SK_Screenshot_SaveJXL  (DirectX::ScratchImage&, const wchar_t*)
                             { return false; }

bool SK_AVIF_CopyToClipboard  (const wchar_t*)                                    { return false; }
bool SK_PNG_CopyToClipboard   (const DirectX::Image&, const void*, unsigned int)  { return false; }
bool SK_HDR_ConvertImageToPNG (const DirectX::Image&, DirectX::ScratchImage&)     { return false; }
bool SK_HDR_SavePNGToDisk     (const wchar_t*, const DirectX::Image*,
                                const DirectX::Image*, const char*)               { return false; }

void SK_WIC_SetLossyQuality   (IPropertyBag2*)          { }
void SK_WIC_SetMaximumQuality (IPropertyBag2*)          { }
void SK_WIC_SetMetadataTitle  (IWICMetadataQueryWriter*, std::string&) { }

float LinearToPQY (float) { return 0.0f; }

const ParamsPQ PQ = { };

void SK_Image_EnqueueTonemapTask (DirectX::ScratchImage&,
                                  std::vector <parallel_tonemap_job_s>&,
                                  std::vector <DirectX::XMVECTOR>&,
                                  float, float)         { }
void SK_Image_InitializeTonemap  (std::vector <parallel_tonemap_job_s>&,
                                  std::vector <void*>&,
                                  std::vector <void*>&) { }
void SK_Image_DispatchTonemapJobs(std::vector <parallel_tonemap_job_s>&) { }
void SK_Image_GetTonemappedPixels(DirectX::ScratchImage&, DirectX::ScratchImage&,
                                  std::vector <DirectX::XMVECTOR>&,
                                  std::vector <void*>&) { }


// ==========================================================================
//  SK_Screenshot and SK_ScreenshotManager class method stubs
// ==========================================================================

SK_Screenshot::SK_Screenshot (bool clipboard_only) :
  ulCommandIssuedOnFrame (0),
  bSaveToDisk            (!clipboard_only),
  bPlaySound             (false),
  bCopyToClipboard       (clipboard_only)
{ }

void SK_Screenshot::sanitizeFilename (bool) { }

void           SK_ScreenshotManager::init         (void)               { }
const wchar_t* SK_ScreenshotManager::getBasePath  (void) const         { return L""; }
bool SK_ScreenshotManager::checkDiskSpace  (uint64_t)           const  { return true;  }
bool SK_ScreenshotManager::copyToClipboard (const DirectX::Image&,
                                            const DirectX::Image*,
                                            const wchar_t*) const      { return false; }
SK_ScreenshotManager::SnippingState
     SK_ScreenshotManager::getSnipState    (void)             const    { return SnippingInactive; }
int  SK_ScreenshotManager::getClipboardFormat (void)                   { return 0; }

SK_ScreenshotManager::screenshot_repository_s&
SK_ScreenshotManager::getRepoStats (bool)
{
  return screenshots;
}


// ==========================================================================
//  SK_HID_PlayStationDevice method stubs
// ==========================================================================

bool SK_HID_PlayStationDevice::write_output_report (bool) { return false; }


// ==========================================================================
//  SK_RenderBackend_V2 method stubs  (excluded reflex.cpp)
// ==========================================================================

bool SK_RenderBackend_V2::isReflexSupported (void) const       { return false; }
bool SK_RenderBackend_V2::setLatencyMarkerNV (NV_LATENCY_MARKER_TYPE) const { return false; }


// ==========================================================================
//  SK_SteamAPIContext OnVarChange  (excluded steam_api.cpp)
// ==========================================================================

bool SK_SteamAPIContext::OnVarChange (SK_IVariable*, void*) { return false; }


// ==========================================================================
//  SK_AppCache_Manager stubs  (excluded appcache.cpp)
// ==========================================================================

int SK_AppCache_Manager::loadDepotCache  (uint32_t) { return 0; }
int SK_AppCache_Manager::storeDepotCache (uint32_t) { return 0; }


// ==========================================================================
//  SK_WASAPI_EndPointManager method stubs  (excluded wasapi.cpp)
// ==========================================================================

void SK_WASAPI_EndPointManager::Activate (void) { }

HRESULT STDMETHODCALLTYPE
SK_WASAPI_EndPointManager::OnDeviceStateChanged (_In_ LPCWSTR, _In_ DWORD)
{ return S_OK; }

HRESULT STDMETHODCALLTYPE
SK_WASAPI_EndPointManager::OnDeviceAdded (_In_ LPCWSTR)
{ return S_OK; }

HRESULT STDMETHODCALLTYPE
SK_WASAPI_EndPointManager::OnDeviceRemoved (_In_ LPCWSTR)
{ return S_OK; }

HRESULT STDMETHODCALLTYPE
SK_WASAPI_EndPointManager::OnDefaultDeviceChanged (_In_ EDataFlow, _In_ ERole,
                                                   _In_ LPCWSTR)
{ return S_OK; }

HRESULT STDMETHODCALLTYPE
SK_WASAPI_EndPointManager::OnPropertyValueChanged (_In_ LPCWSTR,
                                                   _In_ const PROPERTYKEY)
{ return S_OK; }


// ==========================================================================
//  EOS / Galaxy / Xbox stubs
// ==========================================================================

namespace SK {
  namespace EOS {
    void         Init    (bool)  { }
    std::string& AppName (void)
    {
      static std::string _empty;
      return _empty;
    }
  }
  namespace Galaxy {
    void Init (void) { }
  }
}

std::string SK_GetFriendlyAppName (void) { return { }; }


// ==========================================================================
//  NGX / DLSS stubs
// ==========================================================================

bool        SK_NGX_IsUsingDLSS                    (void) { return false; }
bool        SK_NGX_IsUsingDLSS_G                  (void) { return false; }
const char* SK_NGX_DLSS_GetCurrentPerfQualityStr  (void) { return ""; }
const char* SK_NGX_DLSS_GetCurrentPresetStr       (void) { return ""; }
void        SK_NGX_DLSS_GetResolution (int& x, int& y, int& ox, int& oy)
            { x = y = ox = oy = 0; }


// ==========================================================================
//  NvAPI / Reflex stubs
// ==========================================================================

NvAPI_Status __cdecl
NvAPI_D3D_SetLatencyMarker_Detour (__in IUnknown*, __in NV_LATENCY_MARKER_PARAMS*)
{ return NVAPI_OK; }

NvAPI_Status __cdecl
NvAPI_D3D_Sleep_Detour (__in IUnknown*)
{ return NVAPI_OK; }

void SK_VK_HookReflex (void) { }

void SK_VRR_UpdateCachedTimings (SK::Framerate::Limiter*) { }


// ==========================================================================
//  Streamline stubs
// ==========================================================================

sl::Result SK_slUpgradeInterface (void**) { return sl::Result::eErrorFeatureNotSupported; }


// ==========================================================================
//  HDR stubs
// ==========================================================================

void SK_HDR_RunWidgetOnce         (void)       { }
void SK_HDR_SetOverridesForGame   (bool, bool) { }


// ==========================================================================
//  ImGui stubs
// ==========================================================================

bool SK_ImGui_IsItemRightClicked  (void)              { return false; }
bool SK_ImGui_KeybindSelect       (SK_Keybind*, const char*) { return false; }
void __stdcall SK_ImGui_KeybindDialog (SK_Keybind*)   { }

float SK_ImGui::SanitizeFontGlobalScale (float scale) { return scale; }

uint64_t SK_ImGui_GetLastKeybindEditorFrame (void) { return 0; }

void SK_ImGui_Warning          (const wchar_t*)               { }
void SK_ImGui_WarningWithTitle (const wchar_t*, const wchar_t*) { }
void SK_ImGui_ReportModeSwitchFailure (void)                  { }
void SK_ImGui_AdjustCursor     (void)                         { }
void SK_ImGui_CenterCursorOnWindow (void)                     { }
void SK_ImGui_CursorBoundaryConfig (bool)                     { }
void SK_ImGui_DrawGraph_Latency (bool)                        { }

LRESULT __stdcall SK_ImGui_KeyboardProc (int, WPARAM, LPARAM) { return 0; }
LRESULT __stdcall SK_ImGui_MouseProc    (int, WPARAM, LPARAM) { return 0; }

LRESULT __stdcall SK_Input_LowLevelKeyboardProc (int, WPARAM, LPARAM) { return 0; }

void SKIF_ImGui_PushDisableState (void) { }
void SKIF_ImGui_PopDisableState  (void) { }


// ==========================================================================
//  XInput / HID stubs
// ==========================================================================

bool  __stdcall SK_XInput_PulseController       (int, float, float) { return false; }
bool  __stdcall SK_XInput_WasLastPollSuccessful (int)               { return false; }
void  __stdcall SK_XInput_ZeroHaptics           (int)               { }
DWORD WINAPI    SK_XInput_GetBatteryInformation (DWORD, BYTE, XINPUT_BATTERY_INFORMATION*)
                                                { return ERROR_DEVICE_NOT_CONNECTED; }
DWORD WINAPI    SK_XInput_PowerOff              (DWORD)             { return 0; }
bool            SK_Input_PreHookWinMM           (void)              { return false; }


// ==========================================================================
//  Diagnostics / Debugger stubs
// ==========================================================================

namespace SK {
  namespace Diagnostics {
    namespace Debugger {
      bool Allow        (bool) { return false; }
      void SpawnConsole (void) { }
      BOOL CloseConsole (void) { return TRUE; }
    }
  }
}

bool SK_Debug_IsCrashing (void) { return false; }

HINSTANCE* SK_Debug_LoadHelper (void) { return nullptr; }

ULONG SK_GetSymbolNameFromModuleAddr (HMODULE, uintptr_t, gsl::span <char> pszOut,
                                      ULONG ulLen)
{
  if (!pszOut.empty () && ulLen > 0) pszOut[0] = '\0';
  return 0;
}


// ==========================================================================
//  SEH / exception stubs  (excluded debug_utils.cpp)
// ==========================================================================

SK_SEH_PreState SK_SEH_ApplyTranslator (_se_translator_function pfn)
{
  return { pfn, 0, nullptr, { } };
}

_se_translator_function SK_SEH_RemoveTranslator (SK_SEH_PreState pre_state)
{
  _set_se_translator (pre_state.pfnTranslator);
  return pre_state.pfnTranslator;
}

void SK_SEH_LogException (unsigned int, _EXCEPTION_POINTERS*, void*) { }

extern "C" NTSTATUS NTAPI
SK_NtLdr_LockLoaderLock (ULONG, ULONG*, ULONG_PTR*) { return 0; }

extern "C" NTSTATUS NTAPI
SK_NtLdr_UnlockLoaderLock (ULONG, ULONG_PTR) { return 0; }


// ==========================================================================
//  Process / thread termination stubs  (excluded debug_utils.cpp)
// ==========================================================================

void __stdcall SK_ExitProcess    (UINT uExitCode)      { ExitProcess (uExitCode); }
void __stdcall SK_SetLastError   (DWORD dwErrCode) noexcept { SetLastError (dwErrCode); }

int __stdcall SK_TerminateProcess (UINT uExitCode)
{
  TerminateProcess (GetCurrentProcess (), uExitCode);
  return 0;
}

int __stdcall SK_TerminateProcess (HANDLE hProcess, UINT uExitCode)
{
  TerminateProcess (hProcess, uExitCode);
  return 0;
}

int __stdcall SK_TerminateThread (HANDLE hThread, ULONG dwExitCode)
{
  TerminateThread (hThread, dwExitCode);
  return 0;
}


// ==========================================================================
//  Utility stubs
// ==========================================================================

DWORD SK_GetCurrentMS (void) noexcept
{
  return static_cast <DWORD> (GetTickCount64 () & 0xFFFFFFFF);
}

bool SK_IsModuleLoaded (const wchar_t* wszModule)
{
  return GetModuleHandleW (wszModule) != nullptr;
}

std::wstring SK_SYS_GetInstallPath (void) { return { }; }

std::vector <unsigned int> const& SK_CPU_GetLogicalCorePairs (void)
{
  static std::vector <unsigned int> _empty;
  return _empty;
}

bool SK_CPU_TestForMWAITX (void) { return false; }

void SK_GetSystemInfo (LPSYSTEM_INFO lpSystemInfo)
{
  if (lpSystemInfo) GetSystemInfo (lpSystemInfo);
}

void SK_CountIO (io_perf_t&, double) { }

void SK_Network_EnqueueDownload (sk_download_request_s&&, bool) { }

void SK_Display_ResolutionSelectUI (bool) { }


// ==========================================================================
//  GPU / present-mon stubs  (excluded gpu_monitor.cpp / presentmon.cpp)
// ==========================================================================

gpu_sensors_t* SK_GPU_CurrentSensorData (void) { return nullptr; }

void SK_PollGPU              (void)        { }
void SK_SpawnPresentMonWorker (void)       { }

namespace SK_PresentMon {
  void DisableProviders (uint64_t) { }
}


// ==========================================================================
//  WinRing0 stubs  (excluded cpu_widget.cpp)
// ==========================================================================

bool SK_WR0_Init      (void) { return false; }
void SK_WR0_Deinit    (void) { }
void SK_WinRing0_Unpack (void) { }


// ==========================================================================
//  VRR / thread-profiler stubs
// ==========================================================================

void SK_Widget_InvokeThreadProfiler (void) { }

void SK_Memory_RemoveHooks (void) { }


// ==========================================================================
//  7z decompress — 2-arg callback overload  (excluded 7zip.cpp)
// ==========================================================================

long SK_Decompress7zEx (const wchar_t*, const wchar_t*,
                        int (__stdcall*)(int, int)) { return 0; }


// ==========================================================================
//  dbghelp stubs — SymLoadModule  (x86 stdcall @24)
// ==========================================================================

DWORD64 IMAGEAPI SymLoadModule (HANDLE, HANDLE, PCSTR, PCSTR, DWORD, DWORD)
{ return 0; }


#endif // SK_SIDECAR_MINIMAL
