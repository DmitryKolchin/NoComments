## Pre Production

### Planning the proof of concept prototypes

For the October I've planned finishing two prototypes - Combat Prototype and Stealth System Prototype. For better organisation of the project I created a ClickUp workspace for up-to date task tracking, so I don't need to keep it all in my head:

<iframe class="clickup-embed" src="https://sharing.clickup.com/9015630444/c/h/8cnz6kc-115/69343aec5fbc962" onwheel="" width="100%" height="700px" style="background: transparent; border: 1px solid #ccc;"></iframe>

Basically, there are two milestones - stealth prototype and combat prototype. 
Stealth prortype has following subtasks:
- Implementing switching between standing and crouching for the player
- Add player detection to the enemy (by sight)
- Implement levels of enemy awareness of the player 
- Add invisibility zone 
- Utilise PCG to add grass to invisibility zone

Combat Prototype has following subtasks:
- Implement hit
- Implement block
- Implement attacking NPC behavior
- Camera Soft-Lock on the character

Task "Research State Tree" is shared for both of the prototypes since the state tree system is required in both of the prototypes.



### Importing  default Unreal Skeletal Meshes

Since the prototype does not require any complicated visuals, I decided to put default Unreal's skeletal meshes to the project so I can use them for both player and NPC characters

![](./Resources/PreProduction/Manny.png)
![](./Resources/PreProduction/Quinn.png)

# Setting up basic controllable character

Using [Enhanced Input](https://dev.epicgames.com/documentation/en-us/unreal-engine/enhanced-input-in-unreal-engine) to setup the character basic character inputs. The Enhaned Input is the new system which is currently replacing the deprecated previous input implementation. The capability of adding and removing Mapping Contexts will allow me to imlement logic within character for both Combat and Stealth inputs without manually writing code to disable them. For instance, i do not need combat inputs logic within the stealth section, so I will be able to just remove the combat mapping context and add the stealth mapping context.
For now I created a separate mapping context for default actions - walking and rotating the camera. To avoid code duplication, I separated these inputs into a distinct Mapping Context so I can have it within both prototypes.



