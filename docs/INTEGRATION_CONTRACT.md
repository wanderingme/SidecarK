# INTEGRATION_CONTRACT.md

## Canonical Mapping Name

Local\\SidecarK_Frame_v1_<target_pid>

<target_pid> = injected target process PID (swapchain owner).
Not host PID. Not producer PID.

Mapping name is fixed and versioned.
Any change requires explicit version bump.

## Mapping Ownership

Host:
- Creates mapping once per injection session.
- Binds mapping to injected PID.
- Keeps mapping alive for duration of injection session.
- May recreate mapping only when starting a new injection session.

Host must NOT recreate mapping due to frame staleness.

Consumer:
- Opens mapping only.
- Never creates mapping.
- Never recreates mapping.
- Never writes to mapping.

Producer:
- Opens mapping only.
- Fails fast if mapping missing.
- Never modifies header after initialization.

## Frame Format (SKF1 v1)

Offset | Field
-------|------
0x00   | 'SKF1' (uint32)
0x04   | version = 1
0x08   | header_bytes = 0x20
0x0C   | data_offset = 0x24
0x10   | pixel_format = 1 (BGRA8)
0x14   | width
0x18   | height
0x1C   | stride
0x20   | frame_counter (volatile uint32)
0x24   | pixel data begins

Stride must be >= width * 4.

## Producer Write Rules

1. Write pixel data first.
2. Issue memory barrier if applicable.
3. Increment frame_counter last (release semantics).

## Consumer Read Rules (Stable Read)

1. Read frame_counter (c1).
2. Copy pixel data into GPU upload buffer.
3. Read frame_counter again (c2).
4. Accept frame only if:
   - c1 == c2
   - c1 != 0

If unstable:
- Discard upload.
- Retain last valid frame.

## Staleness Behavior

If producer stops updating:
- Consumer retains last valid frame.
- Consumer does NOT recreate mapping.
- Consumer does NOT blank frame automatically.

## Backend Parity Requirement

All supported backends must:

- Use identical mapping name construction.
- Validate identical header layout.
- Derive counter from (data_offset - 4).
- Enforce stable-read semantics.
- Retain last-good-frame behavior.
- Never mutate mapping memory.

Backend differences are restricted strictly to GPU upload/composite mechanics.

Vulkan is not implemented and not part of this contract.
## Phase 3: UI Surface Producer (No Protocol Change)

Phase 3 replaces the test-producer visuals with a real UI surface producer.

Contractual invariants:
- Mapping name is unchanged.
- SKF1 header layout is unchanged.
- Pixel format remains BGRA8 (pixel_format=1).
- Consumer/compositor code is unchanged.
- No new cross-process synchronization primitives are introduced.

Producer behavior additions (allowed):
- Producer may skip frame publication when UI is unchanged (frame_counter does not advance).
- Producer may rate-limit output.

Alpha semantics:
- Producer writes BGRA8 including A channel.
- Consumer/compositor behavior is authoritative; Phase 3 must not require consumer changes.
- Producer must therefore match the alpha convention already used by the compositor in Phase 2 (no change implied by Phase 3).

Control plane:
- Any producer runtime controls (on/off/fps/quit) must be implemented out-of-band from SKF1.
- No additional fields may be added to the shared mapping.
### Control Client Boundary (Explicit)

A separate control client may exist.

It must:
- Never open/read/write the SKF1 mapping.
- Never derive state by inspecting shared memory.

It must:
- Communicate only with the producer process via an out-of-band channel (stdin/pipe/socket/etc).
