# AGENT_RULES.md

## Scope Constraints

- Do not modify injector/bootstrap logic.
- Do not refactor unrelated subsystems.
- No speculative refactors.
- No placeholder implementations.
- One task per PR.
- Minimal diffs only.
- Must compile cleanly in Visual Studio solution.

## Architectural Invariants

- Mapping name: Local\\SidecarK_Frame_v1_<target_pid>
- Consumer never creates mapping.
- Producer never creates mapping.
- Host is sole mapping owner.
- Header layout is fixed (SKF1 v1).
- Counter is derived from (data_offset - 4).
- Stable-read semantics are mandatory.

## API Focus

Phase 1:
- DXGI / D3D11 render path validation.
- Static frame ingestion validation.

Phase 2:
- Continuous producer-fed streaming.
- Stable-read enforcement.
- Last-good-frame retention.
- Streaming validated across all supported GPU backends.

Supported Backends:
- DXGI / D3D11
- DXGI / D3D12
- OpenGL / WGL

Not Implemented:
- Vulkan

All supported backends must consume the identical SKF1 protocol.
Backend differences are restricted strictly to upload/composite mechanics.
No backend may diverge from protocol semantics.
## Phase 3 Rules

**Phase 3 definition:** External UI frame producer publishes BGRA frames into the existing SKF1 mapping; SidecarK consumer/compositor unchanged.

Phase 3 work is limited to implementing a real producer (UI → BGRA frame stream) while keeping Phase 2 consumer/compositor untouched.

Allowed changes:
- Producer subprocess implementation details.
- Producer control plane (out-of-band from SKF1): on/off/fps/quit/clear.

Forbidden changes:
- Any modifications to SKF1 header layout, offsets, or version.
- Any modifications to injected consumer/compositor paths for the sake of the producer.
- Any new shared-memory fields or side-channel backchannels.
- Any blocking waits/sleeps added to Present/composite paths.
- Any `.cpp`, `.h`, `.rc`, `.def`, or build script changes (e.g., `.vcxproj`, `.sln`, `Makefile`, `CMakeLists.txt`).

### Phase 3 Explicit Guardrails

These rules are non-negotiable. Reject any PR that violates any of the following:

1. **No new shared memory fields.** The SKF1 header layout is frozen at Phase 2. No field additions, removals, or reorderings are permitted.
2. **No new sync primitives.** No mutexes, events, semaphores, or any other cross-process synchronization objects may be introduced.
3. **Producer is open-only.** The producer must call `OpenFileMappingW` and must never call `CreateFileMappingW`. Fail fast if mapping is absent.
4. **`frame_counter` is last write.** Pixel data must be fully written before `frame_counter` is incremented (release/barrier semantics required).
5. **Control client never touches shared memory.** The control client must not open, read, or write the SKF1 mapping under any circumstances. It communicates exclusively with the producer via an out-of-band channel (stdin/pipe/socket/etc).
6. **SidecarK is docs-only in Phase 3.** Reject any PR that touches C/C++ source, headers, resource files, or build scripts in this repository.
