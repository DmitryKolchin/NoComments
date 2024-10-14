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

## Combat System 
### Research

From a narrative perspective, all the combat encounters in the *No Comments* beta version are one-on-one, close-quarter battles within mosh pits, surrounded by a cheering crowd. This setting inspired me to look towards *Mafia 2*, specifically its prison sequence, as a key reference for the combat system:

<iframe width="560" height="315" src="https://www.youtube.com/embed/ihEJRwTLGEc?si=SERZ-Vp1orE_BfYb&amp;start=573" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

In this sequence, the combat actions are straightforward yet engaging, consisting of the following moves:
- Block
- Dodge
- Light Attack
- Heavy Attack

These moves are available to both the player and NPCs, creating a balanced and engaging gameplay experience. This simple yet effective system fits our initial concept, allowing us to create intense and dynamic fights.

Additionally, the player character is soft-locked onto the enemy, making it easier to target and maintain focus in combat. This feature enhances the fluidity of the fighting mechanics, ensuring a more accessible experience for players.

The original vision for the combat system is to be "easy to learn, hard to master," where each fight feels like a challenge without being overwhelming at first. To lay the groundwork for this system, we’ve broken it down into the following core elements for initial implementation:
- Block
- Light Attack (Jab)
- Soft Lock onto Enemy

These mechanics will be accessible to both the player and NPCs. As development progresses, we plan to expand the system, introducing dodges, heavy attacks, and potentially a combo system to increase depth and complexity.

#### Additional References

We’ve also explored alternative combat systems for inspiration:

- *GTA V*: The combat system in *GTA V* is simple yet challenging, relying on dodges, blocks, and light/heavy attacks, providing a satisfying balance for player engagement.
  
<iframe width="560" height="315" src="https://www.youtube.com/embed/P2lwi1qLLkM?si=4-faYfHt_akD4oTB&amp;start=573" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

- *Sifu*: This game offers a more intricate combat system, featuring a variety of combos, blocks, and crowd control mechanics. While it’s likely too complex for our student project, its approach to multiple-enemy combat is something we might explore in future iterations.

<iframe width="560" height="315" src="https://www.youtube.com/embed/tGtr_qJDQXw?si=nfeEpo8A0HQt21Db&amp;start=573" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

This evolving framework will help shape our combat system, ensuring that it remains manageable yet open to future enhancements.
Here's the refactored markdown document with more depth and clarity, while addressing logical and grammatical errors:

### Architecture Design

Before implementing the combat system, I decided to spend some time carefully designing the system architecture. Although we are currently working on a proof-of-concept prototype, developing a scalable design from the outset will save significant time in the future.

Here’s the architecture I’ve designed:

The combat system can be split into two main parts:
- **Player Combat**
- **AI Combat**

However, both parts share a significant amount of functionality, such as:
- Moving towards the opponent
- Entering/Exiting a blocking state: The character plays a blocking animation, reducing damage significantly. The damage reduction percentage should be an exposed parameter, allowing easy adjustment during balancing. The character can still move while blocking.
- Performing punches (left jab/right jab): Animations that deal damage to the opponent if they successfully land.
- Soft-locking to the opponent for easier targeting and smoother combat.

**Player Combat** is fully controlled by player input. This means that attacks, blocks, and movement are triggered exclusively by key presses.

**AI Combat**, in contrast, derives its inputs from an AI State Tree, rather than from a player’s keyboard or controller.

To avoid duplicating shared logic between the Player and AI characters, I devised the following architecture:

![](./Resources/Combat%20System/Architecture.jpg)

Let me explain the components in more detail:

#### Combat Component
This component, which is central to the entire system, handles the shared logic between Player Combat and AI Combat. As per the [Unreal Engine 5.5 Documentation](https://dev.epicgames.com/documentation/en-us/unreal-engine/components-in-unreal-engine), components are ideal for encapsulating common behaviors that can be shared between different actors. The **Combat Component** provides a public interface and implements all the core functionality. Here’s a breakdown of its key functions:
- `LeftHandJab()` and `RightHandJab()` trigger the respective punch animation montage.
- `StartBlock()` and `QuitBlock()` activate and deactivate the blocking state for the owning character.
 These functions modify a boolean variable within the component. 
 To reduce coupling, I’ve designed the Animation Blueprint to read this boolean directly from the Combat Component.
  This way, if we ever need to change the skeleton, we won’t have to modify the logic inside the Combat Component itself.

#### Damage Dealing Sphere Component
The **Damage Dealing Sphere Component** is spawned by the Combat Component whenever an attack montage begins. It manages its own lifetime and self-destructs when the animation montage ends. This eliminates the need for additional variables in the Combat Component to handle the sphere’s destruction. The Damage Dealing Sphere holds the attack's damage value and applies it to any entity it overlaps. This approach is preferable to using an Animation Notify State to spawn a default sphere component because passing damage values to the Anim Notify State would require more complex logic (e.g., saving the value in the Combat Component and then retrieving it in the Anim Notify State).

#### Combat Animations Data Asset
A **Data Asset** stores information related to a specific system, allowing for easy scalability and 
customization ([Unreal Engine 5.5 Documentation](https://dev.epicgames.com/documentation/en-us/unreal-engine/data-assets-in-unreal-engine)).
The **Combat Animations Data Asset (DA)** stores all the animations for each attack. By using a Data Asset, I can easily create multiple configurations for different characters or combat styles. This flexibility allows us to switch between different animation sets with a single click, making the system highly scalable. As new features like dodges and heavy attacks are introduced, the system can be easily extended to accommodate them.
Keeping all the animations in one place also makes it easier to change them as we update our project.
This architecture ensures that our combat system is both efficient and adaptable, 
paving the way for future enhancements without unnecessary complexity.
### Implementation
#### Combat Component
BP_CombatComponent is a blueprint actor component. Due to the fact that currently I am working on the prototype, it is crucial to make the workflow as fast as possible, so the blueprints are my choice.


#### Damage Dealing Sphere Component

### Importing and retargeting animations
Importing bought

Retarget

Setup anim bp to merge bones

### Setting up character inputs

Stuff about setting up fight inputs

### Testing 


# Declared Assets

[Animations](https://www.unrealengine.com/marketplace/en-US/product/fighting-animations)


