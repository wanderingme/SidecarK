# GOALS.md

## Project Identity

SidecarK is a SpecialK fork consisting of:

- A host component (injection + lifecycle management)
- An injected rendering component (overlay composite path)
- A producer that feeds frames via shared memory

## Architectural Lock-In

1. Host is the sole mapping owner.
2. Consumer must never create or recreate the shared memory mapping.
3. Producer must fail fast if mapping is absent.
4. Injector/bootstrap logic is infrastructure and must not be refactored.
5. One PR per task. Minimal diffs only.

## Phase Definitions

Phase 1:
- Shared memory ingest validated.
- Static BGRA test frame visible.
- Composite confirmed.
- No mapping churn.
- No injector crash.
- Deterministic visibility toggle.

Phase 2:
- Continuous producer-fed streaming.
- Stable-read semantics enforced.
- Frame_counter monotonic correctness.
- Last-good-frame retained on staleness.
- Producer stop/start handled cleanly.
- No silent failure.
- Streaming validated across all supported backends.

## Backend Coverage

Supported:
- DXGI / D3D11
- DXGI / D3D12
- OpenGL / WGL

Not Implemented:
- Vulkan

Backend parity is required across supported paths.
Protocol semantics must remain identical.
Composite implementation may differ, protocol may not.
## Phase 3
### Phase 3 Scope Lock (Repo Split)

Phase 3 implementation work happens in the external Frame Producer / Control Client repos.

SidecarK Phase 3 rule:
- SidecarK code changes are forbidden (docs-only).
- Consumer/compositor remains Phase 2-complete.
- SKF1 ABI and mapping name remain unchanged.

Phase 3:
- Replace test-producer visuals with a real overlay surface producer (UI → BGRA frame stream).
- Producer remains a separate subprocess and is backend-agnostic.
- Producer writes BGRA8 frames into the existing SKF1 mapping only.
- Consumer/compositor code remains untouched.
- SKF1 ABI remains unchanged (no version bump).
- Producer may skip frame publication when UI is unchanged (counter does not advance).
- No new cross-process synchronization primitives.

Phase 3 completes the SidecarK subprocess by making the producer generate real UI frames instead of a seeded test frame.
