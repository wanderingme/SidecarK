# PRODUCER_INTERFACE.md

## Responsibilities

Producer writes frames into mapping created by Host.

Producer must:
- Open mapping using canonical name.
- Validate SKF1 header.
- Write pixel data before incrementing frame_counter.
- Increment frame_counter last.
- Never modify header after initialization.

Producer must NOT:
- Create mapping.
- Recreate mapping.
- Modify mapping ownership.
- Implement backend-specific logic.

## Failure Behavior

If mapping does not exist:
- Fail fast.
- Do not create mapping.

## Backend Independence

Producer is fully backend-agnostic.

Producer does not:
- Detect GPU backend.
- Modify write semantics per backend.
- Change protocol based on consumer path.

All supported GPU backends consume identical frame protocol.
## Phase 3: UI Surface Producer

Phase 3 replaces the Phase 1 test frame with a real UI surface producer.

Producer MUST:
- Generate a BGRA8 surface of exactly (width × height) described by SKF1 header.
- Respect stride exactly (row-by-row copy using stride).
- Publish frames using the strict ordering rules:
  1) write pixels
  2) memory visibility barrier (as applicable)
  3) increment frame_counter last
- Remain backend-agnostic (no D3D/DXGI/GL detection).
- Never block the injected render thread (producer is a separate process; avoid pathological CPU / busy loops).

Producer MAY:
- Skip publishing (do not increment frame_counter) when UI content is unchanged.
- Rate-limit output (e.g., target 60 FPS default) as long as semantics remain correct.

Producer MUST NOT:
- Change SKF1 header layout or values.
- Add new fields to shared memory.
- Introduce a producer→consumer backchannel.

## Control Plane (Producer Runtime Controls)

Control plane is separate from SKF1 and must not modify the SKF1 mapping format.

Minimum controls required for Phase 3:
- Enable/disable publishing (on/off)
- Set target FPS (rate limit)
- Clear/blank once (write a frame once, then stop incrementing unless re-enabled)
- Quit

Transport is implementation-defined (CLI flags, stdin, named pipe). Whatever is chosen must not require any changes to the consumer/compositor.
