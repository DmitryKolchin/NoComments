#pragma once

#include "CoreMinimal.h"
#include "CharacterCombatState.generated.h"

UENUM(BlueprintType)
enum class ECharacterCombatState : uint8
{
	None,
	NotInCombat UMETA(DisplayName = "Not In Combat"),
	Idle,
	Attacking,
	Blocking,
	Stunned,
	KnockedOut,
};