#pragma once

#include "RepulsionSettings.h"
#include "RepulsionSourceData.generated.h"

UENUM(BlueprintType)
enum class ERepulsionType : uint8
{
	/** Default enum value to check if the value is set */
	None,

	/** The actor is repelled by other actors. */
	ActiveRepulsion,

	/** The actor is unaffected by repulsion forces but remains stationary. */
	PassiveRepulsion
};

/** Information about a source of repulsion. */
USTRUCT(BlueprintType)
struct FRepulsionSourceData
{
	GENERATED_BODY()

	/** The actor generating the repulsion effect. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* SourceActor;

	/** Specifies how the actor interacts with repulsion forces. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERepulsionType RepulsionType;

	/** Configuration settings for this repulsion source. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRepulsionSettings RepulsionSettings;
};
