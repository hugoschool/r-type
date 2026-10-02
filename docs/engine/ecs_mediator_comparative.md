# ECS and Mediator comparative study

## ECS

Splits program design into data containers (Components), IDs (Entities), and logic executors (Systems).

## Mediator

Forces objects to communicate through a single central hub (the mediator) instead of talking to each other directly.

---

### Comparative table

|                           | ECS                             | Mediator                       |
| ------------------------- | ------------------------------- | ------------------------------ |
| Performance               | 🟢 Very high                    | 🟢 Good                        |
| Memory                    | 🟢 Good                         | 🟢 Good                        |
| Scalability               | 🟢 Excellent for entities       | 🟡 Depends on communication    |
| Maintainability           | 🟢 Good                         | 🟡 Can degrade with complexity |
| Flexibility               | 🟢 Very high                    | 🟢 High                        |
| Project suitability       | 🟢 Very suitable for entity-heavy projects              | 🟡 Less suitable for entity-heavy projects               |

Performance -> How fast the pattern can execute operations, especially when there are many entities or interactions.

Memory -> How much RAM the pattern and its data structures require.

Scalability -> How well it continues to work as the number of entities, systems, or interactions increases.

Maintainability -> How easy it is to modify, debug, and extend the code as the project grows.

Flexibility -> How adaptable the system is to different types of entities, behaviors, or requirements.

Project suitability -> How appropriate the pattern is for this project.

