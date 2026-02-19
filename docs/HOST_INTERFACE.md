# HOST_INTERFACE.md

## Responsibilities

Host is sole mapping owner.

Host must:
- Create mapping: Local\\SidecarK_Frame_v1_<target_pid>
- Use CreateFileMappingW with PAGE_READWRITE.
- Bind mapping to injected target PID.
- Keep mapping alive for duration of injection session.

Host must NOT:
- Modify frame protocol.
- Write pixel data.
- Recreate mapping due to staleness.
- Implement backend-specific mapping behavior.

## Backend Neutrality

Mapping lifecycle is independent of rendering backend.

Mapping ownership is bound solely to injected target PID.
Mapping lifetime does not vary by GPU API.

No backend-specific mapping logic is permitted.

## Backend Coverage

Supported:
- DXGI / D3D11
- DXGI / D3D12
- OpenGL / WGL

Not Implemented:
- Vulkan
## Phase 3

Phase 3 does not change Host responsibilities.

Host remains:
- Sole mapping owner.
- Responsible for header initialization.
- Responsible for mapping lifetime.

Host does NOT:
- Render UI.
- Publish frames.
- Coordinate producer cadence.
- Modify SKF1 ABI.
