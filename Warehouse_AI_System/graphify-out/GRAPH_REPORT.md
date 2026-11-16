# Graph Report - Warehouse_AI_System  (2026-11-16)

## Corpus Check
- 4 files · ~2,526 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 2 file(s) not represented in the graph (top: .ino 1, .zip 1)

## Summary
- 32 nodes · 31 edges · 5 communities (2 shown, 3 thin omitted)
- Extraction: 100% EXTRACTED · 0% INFERRED · 0% AMBIGUOUS
- Token cost: 0 input · 0 output

## Community Hubs (Navigation)
- upload_to_edge_impulse.py
- SystemState
- Project: Automated Warehouse AI Object Sorting System Architecture

## God Nodes (most connected - your core abstractions)
1. `SystemState` - 7 edges
2. `Project: Automated Warehouse AI Object Sorting System Architecture` - 6 edges
3. `upload_dataset()` - 3 edges
4. `1. System Overview & Architecture` - 1 edges
5. `2. Hardware Pin Allocation & Wiring Table` - 1 edges
6. `3. Finite State Machine (FSM) Control Flow` - 1 edges
7. `4. Grading Rubric Compliance (Level 4 Verification)` - 1 edges
8. `5. Plug-and-Play File Locations` - 1 edges
9. `==============================================================================…` - 1 edges
10. `==============================================================================…` - 1 edges

## Surprising Connections (you probably didn't know these)
- None detected - all connections are within the same source files.

## Import Cycles
- None detected.

## Communities (5 total, 3 thin omitted)

### Community 2 - "SystemState"
Cohesion: 0.29
Nodes (7): SystemState, STATE_CONVEYOR_RUNNING, STATE_IDLE, STATE_INFERENCE, STATE_OBJECT_DETECTED, STATE_SORTING_ACTION, STATE_TRANSMIT_CLOUD

### Community 4 - "Project: Automated Warehouse AI Object Sorting System Architecture"
Cohesion: 0.29
Nodes (6): 1. System Overview & Architecture, 2. Hardware Pin Allocation & Wiring Table, 3. Finite State Machine (FSM) Control Flow, 4. Grading Rubric Compliance (Level 4 Verification), 5. Plug-and-Play File Locations, Project: Automated Warehouse AI Object Sorting System Architecture

## Knowledge Gaps
- **6 isolated node(s):** `STATE_CONVEYOR_RUNNING`, `STATE_IDLE`, `STATE_INFERENCE`, `STATE_OBJECT_DETECTED`, `STATE_SORTING_ACTION` (+1 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 23 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **3 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `SystemState` connect `SystemState` to `Warehouse_AI_Sorter.cpp`?**
  _High betweenness centrality (0.110) - this node is a cross-community bridge._
- **What connects `STATE_CONVEYOR_RUNNING`, `STATE_IDLE`, `STATE_INFERENCE` to the rest of the system?**
  _6 weakly-connected nodes found - possible documentation gaps or missing edges._