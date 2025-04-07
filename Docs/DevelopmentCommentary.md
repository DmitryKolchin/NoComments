# Development Commentary Template

## Project Outline

**No Comments** is a third-person action game set in the underworld of low-level UK criminal activity. The narrative follows a single protagonist across a timeline stretching from school years to an unavoidable, consequential end. While stylistically inspired by the *Grand Theft Auto* franchise, the game eschews the open-world format in favour of more focused, linear gameplay combined with deeper, more punishing mechanics. A core feature is the implementation of hidden "stats" that are influenced by player decisions, creating branching gameplay outcomes and affecting future events.

For instance, the result of the first in-game fight (win or lose) will dynamically adjust the player's “Strength” parameter. A victory may boost this stat, potentially making future encounters easier, while a loss could reduce it, increasing difficulty. Importantly, these stats remain hidden from the player, encouraging immersive, in-world decision-making rather than number crunching.

---

### Minimal Goal – Fully Polished Fight Level

The fighting system forms the backbone of *No Comments* and is currently the central focus of development. The minimal deliverable for the project is a fully functional and polished combat level, complete with engaging and responsive brawling mechanics.

#### Key Objectives:
- Design a compact but immersive level centred around hand-to-hand combat.
- Develop enemy AI that presents a fair but challenging opponent.
- Implement responsive controls and impactful animations to convey weight and momentum.

#### Anticipated Challenges:
- Creating intelligent AI that adapts to player behaviour without becoming predictable.
- Ensuring that punches and movement feel “heavy” and satisfying through a combination of sound, visual feedback, and animation timing.
- Delivering fluid, context-aware animations that support both combat mechanics and the game's overall tone.

---

### Desired Goal – Stealth Integration

A stealth level is a secondary goal for the project, intended to provide variety and additional depth. The intention is to allow players the choice between silent infiltration and open combat. Ideally, stealth segments will transition smoothly into fights when necessary, depending on player choice or failure.

#### Key Objectives:
- Design a level that encourages exploration and experimentation with stealth mechanics.
- Integrate the fighting system seamlessly with stealth to allow for dynamic gameplay shifts.

#### Anticipated Challenges:
- Balancing stealth and combat to ensure neither approach trivialises the other.
- Managing transitions between stealth and combat states smoothly to avoid breaking immersion.
- Creating AI behaviours that can respond intelligently to both stealthy and aggressive playstyles.

---

### Aspirational Goal – Dynamic Hidden Stats and Event Variety

The hidden stat system is one of the project’s most distinctive features, offering a subtle but significant impact on gameplay and narrative progression. These stats, while not shown explicitly to the player, will shape encounters and potentially alter the path of the story. Although relatively straightforward to implement from a technical perspective, these systems require significant design iteration and testing to balance properly.

#### Key Objectives:
- Introduce at least three hidden parameters (Health, Damage, Intelligence, Stamina, Agility) that evolve based on player's actions.
- Link these stats to gameplay elements in a way that feels organic and meaningful.
- Introduce branching narrative or gameplay events that reflect the player’s stat profile, increasing replayability.

#### Anticipated Challenges:
- Making the impact of hidden stats noticeable without exposing numerical values.
- Ensuring that all gameplay paths remain viable, yet offer distinct challenges and outcomes.
- Avoiding excessive linearity by designing enough variance to encourage multiple playthroughs.

---




## Research

TO BE FILLED LATER

## Implementation

## Tools 

### Metahuman Data Extractor 
#### Motivation

Given that all of the characters in game - player, enemies, NPCS - are going to be Epic's Meta Humans [(MetaHuman | Realistic Person Creator, s.d.)](https://www.unrealengine.com/en-US/metahuman), I was able to foresee a problem arising:
- Be default, when metahuman is imported, UE creates a separate Blueprint with all the skeletal meshes and groom elements setup.
- This means, that we have to use a separate class for each new character. 
- And this creates problem when we want to use all of that within our custom class - we need to manually move all of the skeletal meshes to our blueprint
- And if we still cannot use multiple Meta Humans with one class since all of that skeletal mesh parameters are hardcoded.

In order to address all of those problems, I developed the Meta Human Data Extractor plugin. 

### Extracting and storing the Metahuman Data

To make a convinient way to dynamically loadup metahumans within a single class, I developed a ```UMetahumanComponentsDataAsset```. It has:
- SkeletalMeshComponents - a Map of ```FName``` values to ```USkeletalMeshComponent*```, which stores a name of the component (e.g. Face or Body) as a key and the copy of the component from the source metahuman blueprint as a value
- GroomComponents - analogical map 
- TSoftObjectPtr<UBlueprint> SourceMetahumanBlueprint - soft reference to the source Blueprint of the Meta Human. Making it a soft reference makes it possible to load the Data Asset without loading the source blueprint each time it appears in the content browser.

From the functionality perspective, the most intersting is ```ExtractDataFromMetahumanBlueprint(UObject* Object)``` function - it accepts the Metahuman Blueprint, and fills the data asset with all the required component data. 

Let's dicuss some challenges faced during the implmentation of this part:
#### Magical values

The only way how we can distinguish the Face skeletal mesh from the Body skeletal mesh is by their ```Name```, which is set as a hardcoded value within the imported blueprint. These names - ```Face```, ```Feet```, ```Legs```, ```Torso``` and ```Body``` (similar list for the Groom) - are the same across the whole project, and there are several places I need to access them from. Basically, those are global values, and I needed 

### Process
- Provide a step-by-step breakdown of your development process, including key milestones and decisions made throughout the project.  
- Highlight any tools, frameworks, or techniques used, and explain how they contributed to the implementation.  
- Include screenshots, diagrams, or code snippets where relevant to showcase your progress.

### New Approaches  
- Detail any innovative or new approaches you explored during the project.  
- Explain why these approaches were chosen and how they differ from standard practices.  
- Evaluate the success of these approaches, including any challenges faced and lessons learned.

### Testing
- Document the user testing conducted, specifying the type of tests used (e.g., automated testing, guided user testing, blind testing).  
- Present feedback or issues identified during testing, using graphs, tables, or visual aids to summarise results.  
- Describe how these issues were addressed. If any issues were not resolved, provide a clear justification for leaving them unaddressed.

### Technical Difficulties
- Identify any technical difficulties encountered during the implementation phase.  
- Provide details on how these issues were diagnosed and resolved.  
- If any difficulties remain unresolved, explain the impact on the project and any mitigation strategies used to minimise their effect.  
- Reflect on what you would do differently in future projects to avoid similar issues.

## Outcomes

TO BE FILLED LATER



## Reflection
TO BE FILLED LATER

## Bibliography  

## Declared Assets
Assets generated by ChatGPT:
- DevelopmentCommentary.md