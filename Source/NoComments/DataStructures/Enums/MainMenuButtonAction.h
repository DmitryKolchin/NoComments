#pragma once

#include "MainMenuButtonAction.generated.h"

UENUM(BlueprintType)
enum class EMenuButtonAction : uint8
{
	None,
	FightDemo,
	Options,
	QuitOptions,
	Quit
};