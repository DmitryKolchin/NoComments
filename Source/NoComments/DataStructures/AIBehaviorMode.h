#pragma once

#include "AIBehaviorMode.generated.h"

UENUM(BlueprintType)
enum class AIBehaviorMode : uint8
{
	Fighting,
	Patrolling,
	Default
};

