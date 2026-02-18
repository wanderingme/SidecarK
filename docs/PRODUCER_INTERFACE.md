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