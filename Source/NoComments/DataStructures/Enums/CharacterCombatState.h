#pragma once

#include "CoreMinimal.h"
#include "CharacterCombatState.generated.h"

UENUM(BlueprintType)
enum class ECharacterCombatState : uint8
{
	None,
	/** Not in combat */
	NotInCombat UMETA(DisplayName = "Not In Combat"),
	/** In combat, but not attacking nor blocking */
	Idle,
	/** Performing an attack animation */
	Attacking,
	/** Block is active */
	Blocking,
	/** Stunned by an attack */
	Stunned,
	/** Character lost the fight */
	KnockedOut,
};