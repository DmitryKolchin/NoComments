#pragma once

#include "RepulsionSettings.generated.h"

/** General settings for repulsion */
USTRUCT(BlueprintType)
struct FRepulsionSettings
{
	GENERATED_BODY()

	/** The radius within which the actor is actively repelled by other actors. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActiveRepulsionRadius = 100.f;

	/** The additional radius beyond the active repulsion zone where the actor is neither repelled nor returns to a previous position. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PassiveRepulsionAdditionalRadius = 20.f;

	/** Determines the strength of the repulsion force applied within the active radius. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RepulsionStrength = 100.f;
};