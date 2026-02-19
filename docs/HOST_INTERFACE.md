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

**Definition:** External UI frame producer publishes BGRA frames into the existing SKF1 mapping; SidecarK consumer/compositor unchanged.

Phase 3 does not change Host responsibilities.

Host remains:
- Sole mapping owner.
- Responsible for header initialization (including setting `frame_counter` to zero).
- Responsible for mapping lifetime.

Host does NOT:
- Render UI.
- Publish frames.
- Coordinate producer cadence.
- Modify SKF1 ABI.

## Phase 3 Scope Lock (Docs-Only)

Phase 3 does not permit SidecarK host/consumer code changes.

Only documentation updates are allowed in this repo for Phase 3.

### Phase 3 Explicit Guardrails

The following constraints apply to all Phase 3 work touching this repository:

1. **No new shared memory fields.** The SKF1 header layout written by the host at injection time is frozen. Phase 3 must not add, remove, or reorder any field.
2. **No new sync primitives.** The host must not create any new cross-process synchronization objects (mutexes, events, semaphores) as part of Phase 3.
3. **Producer is open-only.** The producer must never receive a handle from the host; it must independently call `OpenFileMappingW` and fail fast if the mapping is absent.
4. **`frame_counter` is last write.** The host initializes `frame_counter` to zero; only the producer may subsequently increment it, and only after all pixel data has been written with a memory barrier.
5. **Control client never touches shared memory.** Any control client that interfaces with the producer must have zero access to the SKF1 mapping. It must not receive the mapping handle and must not call `OpenFileMappingW`.
