#pragma once

#include "CoreMinimal.h"
#include "OpponentDistanceFromCombatAreaStatus.generated.h"

UENUM( BlueprintType )
enum class EOpponentDistanceFromCombatAreaStatus : uint8
{
	TooFar,
	InsideCombatArea,
	TooClose,
	None
};