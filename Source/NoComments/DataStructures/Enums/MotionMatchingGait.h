#pragma once

UENUM(BlueprintType)
enum class EMotionMatchingGait : uint8
{
	None,
	/** Walking */
	Walking UMETA(DisplayName = "Walking"),
	/** Running */
	Running UMETA(DisplayName = "Running"),
};