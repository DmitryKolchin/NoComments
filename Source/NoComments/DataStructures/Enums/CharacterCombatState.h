#pragma once

#include "CoreMinimal.h"
#include "CharacterCombatState.generated.h"

UENUM(BlueprintType)
enum class ECharacterCombatState : uint8
{
	Idle,
	Attacking,
	Blocking,
};