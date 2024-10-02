## Pre-Production

### Planning Proof of Concept Prototypes

For October, I have outlined the development of two key prototypes: the **Combat Prototype** and the **Stealth System Prototype**. To streamline the workflow and ensure efficient task management, I established a dedicated ClickUp workspace for real-time task tracking. This eliminates the need to track progress  in my head and provides a clear, organised project overview.

<iframe class="clickup-embed" src="https://sharing.clickup.com/9015630444/c/h/8cnz6kc-115/69343aec5fbc962" onwheel="" width="100%" height="700px" style="background: transparent; border: 1px solid #ccc;"></iframe>

The project is divided into two primary milestones: the **Stealth Prototype** and the **Combat Prototype**. Each prototype is further broken down into specific subtasks to ensure a clear development path.

#### Stealth Prototype Subtasks:
- **Player Stance Control**: Implementing mechanics for switching between standing and crouching, giving players the ability to dynamically alter their movement posture.
- **Enemy Detection**: Adding a system where enemies can detect the player based on line of sight.
- **Enemy Awareness Levels**: Creating multiple states of enemy awareness to simulate realistic stealth mechanics.
- **Invisibility Zones**: Introducing zones where the player can remain hidden from enemies, enhancing the stealth gameplay.
- **Procedural Content Generation (PCG) for Grass**: Using PCG to populate invisibility zones with grass, adding a visual cue for players.

#### Combat Prototype Subtasks:
- **Combat Mechanics**: Developing basic hit and block mechanics to lay the foundation for player combat.
- **NPC Combat Behaviour**: Implementing AI for non-player characters (NPCs) to react to player attacks.
- **Camera Soft-Lock**: Integrating a camera system that soft-locks onto targets during combat for a more focused player experience.

The task titled **Research State Tree** is a shared objective across both prototypes, as a unified state tree system is essential for managing the different states in both combat and stealth mechanics.

### Integrating Default Unreal Skeletal Meshes

Given that the prototype phase does not require complex visual assets, I imported Unreal Engine's default skeletal meshes to be used for both player and NPC characters. This approach saves time on asset creation while still providing functional models for testing gameplay mechanics.

![](./Resources/PreProduction/Manny.png)
![](./Resources/PreProduction/Quinn.png)

### Setting Up the Basic Controllable Character

For player character control, I utilised Unreal Engine’s [Enhanced Input](https://dev.epicgames.com/documentation/en-us/unreal-engine/enhanced-input-in-unreal-engine) system. This system, which is replacing the deprecated input implementation, allows for greater flexibility in managing input contexts. By leveraging **Mapping Contexts**, I can efficiently switch between combat and stealth controls without the need for manual input handling.

For example, combat inputs are unnecessary during stealth gameplay. By removing the combat mapping context and adding the stealth mapping context, I can ensure that only relevant inputs are active. To ensure modularity, I created a separate **Mapping Context** for basic actions like movement and camera control. This context is shared across both prototypes to avoid redundant code.

<iframe width="100%" height="500px" src="https://blueprintue.com/render/yz-ogp8q/" scrolling="no" allowfullscreen></iframe>
