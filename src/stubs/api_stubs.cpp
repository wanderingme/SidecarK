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

// ---- HID types (HIDP_REPORT_TYPE, PHIDP_DATA, PHIDP_PREPARSED_DATA)
#include <SpecialK/input/input.h>


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


#endif // SK_SIDECAR_MINIMAL
