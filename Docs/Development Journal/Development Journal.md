## Pre-Production

### Planning Proof of Concept Prototypes

For October, I have outlined the development of two key prototypes: the **Combat Prototype** and the **Stealth System Prototype**. To streamline the workflow and ensure efficient task management, I established a dedicated ClickUp workspace for real-time task tracking. This eliminates the need to track progress  in my head and provides a clear, organised project overview.

<iframe class="clickup-embed" src="https://sharing.clickup.com/9015630444/c/h/8cnz6kc-115/69343aec5fbc962" onwheel="" width="100%" height="700px" style="background: transparent; border: 1px solid #ccc;"></iframe>

*Figure 1. ClickUp project plan layout*

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
*Figure 2. Manny skeletal mesh*  

![](./Resources/PreProduction/Quinn.png)
*Figure 3. Quinn skeletal mesh*

### Setting Up the Basic Controllable Character

For player character control, I utilised Unreal Engine’s [Enhanced Input](https://dev.epicgames.com/documentation/en-us/unreal-engine/enhanced-input-in-unreal-engine) system. This system, which is replacing the deprecated input implementation, allows for greater flexibility in managing input contexts. By leveraging **Mapping Contexts**, I can efficiently switch between combat and stealth controls without the need for manual input handling.

For example, combat inputs are unnecessary during stealth gameplay. By removing the combat mapping context and adding the stealth mapping context, I can ensure that only relevant inputs are active. To ensure modularity, I created a separate **Mapping Context** for basic actions like movement and camera control. This context is shared across both prototypes to avoid redundant code.

<iframe width="100%" height="500px" src="https://blueprintue.com/render/yz-ogp8q/" scrolling="no" allowfullscreen></iframe>

*Figure 4. Enahnced input logic handling*

## Combat System 
### Research

From a narrative perspective, all the combat encounters in the *No Comments* beta version are one-on-one, close-quarter battles within mosh pits, surrounded by a cheering crowd. This setting inspired me to look towards *Mafia II* (Mafia II, 2010), specifically its prison sequence, as a key reference for the combat system:

<iframe width="560" height="315" src="https://www.youtube.com/embed/ihEJRwTLGEc?si=SERZ-Vp1orE_BfYb&amp;start=573" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

*Figure 5. Mafia 2 Remastered- Life In A Prison  (Mafia 2 Definitive Edition Prison Mission 1080p 60fps ) 2020*



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

- *GTA V* (Grand Theft Auto V, 2013): The combat system in *GTA V*  is simple yet challenging, relying on dodges, blocks, and light/heavy attacks, providing a satisfying balance for player engagement.
  
<iframe width="560" height="315" src="https://www.youtube.com/embed/P2lwi1qLLkM?si=4-faYfHt_akD4oTB&amp;start=573" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

*Figure 6. Grand Theft Auto V PS5 - Street Fights With Trevor [4K HDR 60fps] 2022*

- *Sifu* (Sifu, 2022): This game offers a more intricate combat system, featuring a variety of combos, blocks, and crowd control mechanics. While it’s likely too complex for our student project, its approach to multiple-enemy combat is something we might explore in future iterations.

<iframe width="560" height="315" src="https://www.youtube.com/embed/tGtr_qJDQXw?si=nfeEpo8A0HQt21Db&amp;start=573" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

*Figure 7. I went Ultra Instinct for a moment| Sifu PS5 2022*



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

*Figure 8. Class hierarchy scheme*

Let me explain the components in more detail:

#### Combat Component
This component, which is central to the entire system, handles the shared logic between Player Combat and AI Combat. As per the Unreal Documentation [(Components in Unreal Engine | Unreal Engine 5.5 Documentation | Epic Developer Community, s.d.)](https://dev.epicgames.com/documentation/en-us/unreal-engine/components-in-unreal-engine), components are ideal for encapsulating common behaviors that can be shared between different actors. The **Combat Component** provides a public interface and implements all the core functionality. Here’s a breakdown of its key functions:
- `LeftHandJab()` and `RightHandJab()` trigger the respective punch animation montage.
- `StartBlock()` and `QuitBlock()` activate and deactivate the blocking state for the owning character.
 These functions modify a boolean variable within the component. 
 To reduce coupling, I’ve designed the Animation Blueprint to read this boolean directly from the Combat Component.
  This way, if we ever need to change the skeleton, we won’t have to modify the logic inside the Combat Component itself.

#### Damage Dealing Sphere Component
The **Damage Dealing Sphere Component** is spawned by the Combat Component whenever an attack montage begins. It manages its own lifetime and self-destructs when the animation montage ends. This eliminates the need for additional variables in the Combat Component to handle the sphere’s destruction. The Damage Dealing Sphere holds the attack's damage value and applies it to any entity it overlaps. This approach is preferable to using an Animation Notify State to spawn a default sphere component because passing damage values to the Anim Notify State would require more complex logic (e.g., saving the value in the Combat Component and then retrieving it in the Anim Notify State).

#### Combat Animations Data Asset
A **Data Asset** stores information related to a specific system, allowing for easy scalability and 
customization [(Data Assets in Unreal Engine | Unreal Engine 5.5 Documentation | Epic Developer Community, s.d.)](https://dev.epicgames.com/documentation/en-us/unreal-engine/data-assets-in-unreal-engine).
The **Combat Animations Data Asset (DA)** stores all the animations for each attack. By using a Data Asset, I can easily create multiple configurations for different characters or combat styles. This flexibility allows us to switch between different animation sets with a single click, making the system highly scalable. As new features like dodges and heavy attacks are introduced, the system can be easily extended to accommodate them.
Keeping all the animations in one place also makes it easier to change them as we update our project.
This architecture ensures that our combat system is both efficient and adaptable, 
paving the way for future enhancements without unnecessary complexity.
### Implementation
#### Combat Component
BP_CombatComponent is a blueprint actor component. Due to the fact that currently I am working on the prototype, it is crucial to make the workflow as fast as possible, so the blueprints are my choice.


#### Damage Dealing Sphere Component

### Importing and retargeting animations
For the initial animations, I decided to use this asset pack since it was affordable and had everything what i need (punches and blocks) 
included
![](./Resources/Combat%20System/AssetPack.png)

*Figure 9. The asset pack*

The only drawback was that the pack has its own Unreal Mannequin in itself. This mannequin is fully identical to the one I've already
imported to the project, but from the engine's perspective those are 2 different skeletal meshes. So, I was able to make a use of 
this videoguide:

<iframe width="560" height="315" src="https://www.youtube.com/embed/iE474cUpR-o?si=mtvZzMTAhuCWs77t" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

*Figure 10. Retargeting Animations in 5.4 is Finally Easy!!! #unrealengine5 2024*

It turns out that with coming of UE 5.4 retargeting for humnoid rigs became even easier - it can automatically 
detect the bone chains and the suer just need to choose the animations to retarget. In my 
case there were no artifacts since the skeletons are identical.
![](./Resources/Combat%20System/AnimationRetargeter.png)

*Figure 11. Animation retargeter*

And after retargeting all the animations, I removed all the old packs animations to keep the project clean.

### Adding block state to the Anim Blueprint

Before actually jumping into implementing all of the logic, I decided to add the block state support within the animation blueprint.
The block animation has fixed legs - which is not good, since I need the player to be able to move while blocking.
To address this issue, I decided to add the merge per bone node inside the animation blueprint.
How it works - it takes the initial locomotion pose, and blends it with the block animation starting with spine_01 bone.
It takes the result of blend if the ```IsBlockActive``` variable is true and default locomotion otherwise. This is how 
it looks now 

![](./Resources/Combat%20System/Locomotion.png)

### Debugging

In order to make the Blueprint Debugging process easier, I designed a custom function ```Print Debug``` which takes debug message 
and the function name. This will help me identify the problems easier.

<iframe width="100%" height="500px" src="https://blueprintue.com/render/vk0i87ep/" scrolling="no" allowfullscreen></iframe>

### BP_Combat Component
Since I am currently working on the prototype and time is constrained, anything that can be done in blueprints will be done in blueprints
Combat Component does not require any C++ functionality, so it is done via blueprints.

All the attack animations gonna have some shared logic, which is
- Check all the references
- Check that none other montage playing
- Play Montage
- Spawn Damage Dealing Component

The only difference gonna be the animation and root bone to which is Damage Dealing Component attached.
So I decided to incapsulate it all in ```Play Attack Montage``` function:

<iframe width="100%" height="500px" src="https://blueprintue.com/render/cedwnnya/" scrolling="no" allowfullscreen></iframe>

So each of the attack functions gonna look similar to this:

<iframe width="100%" height="500px" src="https://blueprintue.com/render/5nmt71w2/" scrolling="no" allowfullscreen></iframe>


### Combat Animations Data Asset
I cannot inherit from generic UDataAsset class in blueprints, so this class must be declared in C++. Since it does not hold any functionality and serves only as data storage asset, i deleted the .cpp file and kept only .h file in the project.
The class itself looks like this:
```cpp
/**
 * Base class for data assets that store combat related animations
 */
UCLASS( Blueprintable )
class NOCOMMENTS_API UCombatAnimationsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Jabs" )
	TArray<TSoftObjectPtr<UAnimMontage>> RightJabMontages;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Jabs" )
	TArray<TSoftObjectPtr<UAnimMontage>> LeftJabMontages;
};

```

For now it has only 2 arrays for Jabs. But it can be easily scaled later when required.
### Damage Dealing Sphere 
I cannopt inherit blueprint classes from USphereComponent as well, so the Damage Dealing Component is also declared in C++.
Here is the class header:
```cpp
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class NOCOMMENTS_API UDamageDealingSphereComponent : public USphereComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY( meta=(ExposeOnSpawn = true), BlueprintReadOnly  )
	float Damage = 10.0f;

public:
	// Sets default values for this component's properties
	UDamageDealingSphereComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOwnerMontageEnded( UAnimMontage* Montage, bool bInterrupted );

	UFUNCTION()
	void OnBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );
};

```
From interesting details - the Damage value in ```Expose on Spawn``` modifier true, which allowed me to initialize it with the same 
node I created it.

Let's break down the functions:
BeginPlay perfroms a series of checks and then binds to the end of the montage of owning player:
```cpp
// Called when the game starts
void UDamageDealingSphereComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* OwnerCharacter = Cast<ACharacter>( GetOwner() );
	if (!IsValid( OwnerCharacter ))
	{
		ensureAlwaysMsgf( false, TEXT( "DamageDealingSphereComponent must be attached to a character" ) );
		return;
	}

	USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
	if (!IsValid( OwnerMesh ))
	{
		ensureAlwaysMsgf( false, TEXT( "Owner character must have a skeletal mesh component" ) );
		return;
	}

	UAnimInstance* OwnerAnimInstance = OwnerMesh->GetAnimInstance();

	if (!IsValid( OwnerAnimInstance ))
	{
		ensureAlwaysMsgf( false, TEXT( "Owner character must have an anim instance" ) );
		return;
	}


	// Binding self-destruction to the end of the montage
	OwnerAnimInstance->OnMontageEnded.AddDynamic( this, &UDamageDealingSphereComponent::OnOwnerMontageEnded );

	// ...
}
```

The OnOwnerMontageEnded function calls self-destruction:
```cpp
void UDamageDealingSphereComponent::OnOwnerMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	DestroyComponent(  );
}
```
This allows the component to manage its lifetime by itself, without any additional managing from Combat Component. It also does not 
require knowing about the exact player class, just generic character class, which
- reduces coupling
- allows it to be attached to any character child, so can be used on player and NPC even if I do not specify any common parent class for them

Later it can be controlled via Anim Notifies 

It also has overlapping manage to apply damage usign the Unreal's Standart Damage Dealing system:
```cpp
void UDamageDealingSphereComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
	{
		return;
	}

	if (!IsValid( OtherActor ))
	{
		return;
	}

	FDamageEvent DamageEvent;
	OtherActor->TakeDamage( Damage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner() );
}

```

### Testing 
After all the inputs been setup, now the gameplay looks like this
![](./Resources/Combat%20System/)

Here’s a refactored version of the markdown document for improved clarity, depth, and structure:

## AI Behaviour

### Introduction to State Trees

One of my objectives for this project was to become familiar
 with Unreal Engine's new State Trees tool. State
  Trees can be applied to a wide range of cases
   involving stateful data, including AI behaviours.  [(Your First 60 Minutes with StateTree | Tutorial, 2024)](https://dev.epicgames.com/community/learning/tutorials/lwnR/unreal-engine-your-first-60-minutes-with-statetree)

Here’s a summary of the essential terminology from the documentation:
- **Schema** - Sets up use-case specific data and can constrain or expand which nodes may be used within the State Tree.
- **Context** - Data provided by the schema and available to any task or condition in the State Tree.
- **Parameters** - Additional data defined by the user for use within the State Tree. Parameters can have custom values assigned when running the State Tree or linking it through a component like StateTreeComponent.
- **Property Categories** - Metadata for task and condition properties to enable data binding. Key categories include:
  - **Context** - Automatically binds to the best-matching context data but can be overridden.
  - **Input** - A property required by a task/condition that must be bound for the State Tree to compile. Additional public properties can have optional bindings or specified values.
  - **Output** - A property output from a task/condition, bindable as an input for other tasks or conditions. Cannot be bound to parameters.
- **State** - Organizational layers within State Trees that can contain child states, tasks, entry conditions, and transitions.
- **Transition** - Rules defining when a state should switch to another state in the tree.

These elements form the basis of the Combat State Tree for AI.

### AI Decision-Making

Before implementing the State Tree, I designed a conceptual scheme to outline the AI's intended behaviour.

![](./Resources/Combat%20System/AILogicScheme.jpg)

This diagram shows the general flow for AI decision-making within combat.

### Combat State Tree Structure

Here is an overview of the Combat State Tree designed for AI:

![](./Resources/Combat%20System/State%20Tree.png)

Let’s go through each component in detail.

#### Schema

The **Schema** defines Context variables initialized by the State Tree Component instance for the specific actor running the State Tree. Here, I’m using the default State Tree AI Component Schema, which provides access to the AI Controller within State Tree tasks—a sufficient setup for managing in-tree logic.

#### Parameters

All parameters are organized within a separate Data Asset, providing these benefits:
- Balancing adjustments are isolated to a single file, allowing a game designer to modify values without needing to search through the State Tree.
- Commonly-used values (like attack range) are declared in one location, reducing "magic numbers" and improving maintainability.

#### Theme

This section allows for assigning different colours to various states, which visually separates different parts of the State Tree as it grows more complex. While purely cosmetic, it improves readability and navigation.

#### Root State

The **Root** state is the default state selected when the State Tree is activated. It will attempt to transition to a child state, iterating through them until a valid entry condition is met. Since **Blocking** has no entry conditions, the Root state will always transition directly to Blocking.

#### Blocking State

The **Blocking** state contains several tasks:

1. **ST_TaskBlock** (displayed as BP_Block in the State Tree) initiates blocking in the NPC’s Combat Component at the start of the state and ends blocking when the state is complete.
   
   <iframe src="https://blueprintue.com/render/75h3q374/" width="100%" height="500" scrolling="no" allowfullscreen></iframe>

   Events **Enter State** and **State Completed** are triggered by the State Tree as needed, allowing us to define relevant logic.

   To access the AI Controller, we need to mark its variable category as **Context**. Here’s an example of setting this context-based variable:
   
   ![](./Resources/Combat%20System/StateTree/ContextBasedVariable.jpg)

   Now, this variable appears in the State Tree, initialized with the AI Controller context value:

   ![](./Resources/Combat%20System/StateTree/BlockContextInitilization.png)

2. **STTask_WaitRandomTimeInRange** pauses for a random duration and then exits the state, introducing a randomised blocking time. Here’s the task’s code:

   <iframe src="https://blueprintue.com/render/0w30td-v/" width="100%" height="500" scrolling="no" allowfullscreen></iframe>

   This task uses a **Waiting Time Range** parameter, which can be defined within the State Tree or derived from a parameter Data Asset. To make this variable instance-editable, we set its category to **Parameters** and check the **Instance Editable** box:

   ![](./Resources/Combat%20System/StateTree/ParameterVariable.jpg)

   This setup allows us to initialize it with values from the parameter Data Asset or other State Tree parameters:

   ![](./Resources/Combat%20System/StateTree/ParametersInitialization.png)

3. **STTask_GetFirstActorOfClass** finds the first actor of a specified class, in this case, the player character. This task accepts an actor class parameter and outputs a reference to the found actor:

   <iframe src="https://blueprintue.com/render/v00ik7cy/" width="100%" height="500" scrolling="no" allowfullscreen></iframe>

   To set this task’s output parameter, we categorize it as **Output** and check **Instance Editable**:

   ![](./Resources/Combat%20System/StateTree/OutputVariable.jpg)

   This parameter is then available as input for subsequent tasks in this state:

   ![](./Resources/Combat%20System/StateTree/OutputParametersUsage.png)

4. **STTask_MoveToActor** is a custom task based on the Move To node. Unlike the default, this version restarts movement once the target is reached, enabling continuous tracking of the player. Using the default Move To Task was not feasible, as it exits the state upon completion.

The **Blocking** state has two possible transitions:
- **Attack State** if the player is within attack range.
- **Idle State** in all other cases.

#### Idle State

The **Idle** state functions similarly to **Blocking** (excpet for the fact it does not have STTask_Block in it), with the following transition rules:
- Transition to **Blocking** if the opponent is blocking.
- Transition to **Attack** if the opponent is within attack range.
- Transition to **Root** otherwise.
- Can transition to **Stunned** if the **AI.Event.Stunned** tag is applied (e.g., when the NPC takes damage in BP_NPC).

#### Attack State

The **Attack** state contains a single task, **STTask_Attack**, which triggers the attack logic in the NPC's Combat Component. The state transitions to **Idle** once the attack completes.

#### Stunned State

The **Stunned** state is similar to the Attack state. It plays the stunned animation and transitions back to **Blocking** upon completion.

---

This completes the current layout of the Combat State Tree. Here’s the resulting AI behaviour flow:



# Declared Assets

[Animations](https://www.unrealengine.com/marketplace/en-US/product/fighting-animations)


