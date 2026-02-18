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