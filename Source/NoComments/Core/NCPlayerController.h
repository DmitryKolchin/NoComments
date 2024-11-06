// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "NCPlayerController.generated.h"

/**
 *
 */
UCLASS()
class NOCOMMENTS_API ANCPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ANCPlayerController(const FObjectInitializer& ObjectInitializer);


private:
	FGenericTeamId TeamId;

public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

};
