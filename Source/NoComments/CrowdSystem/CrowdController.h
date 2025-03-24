// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrowdAgent.h"
#include "GameFramework/Actor.h"
#include "CrowdController.generated.h"

class ACrowdAgent;
class UMetahumanComponentsDataAsset;
class USplineComponent;

UCLASS()
class NOCOMMENTS_API ACrowdController : public AActor
{
	GENERATED_BODY()
public:
	ACrowdController();
private:
	UPROPERTY(EditAnywhere, Category="Components")
	USceneComponent* CrowdCenterPoint;

	UPROPERTY( EditAnywhere, Category="Components" )
	USplineComponent* SplineComponent;

	UPROPERTY()
	TArray<ACrowdAgent*> CrowdAgents = TArray<ACrowdAgent*>{};

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<ANCCharacter_Base> CrowdAgentClass = ACrowdAgent::StaticClass();

	UPROPERTY(EditAnywhere)
	int32 AgentsNum = 10;

	UPROPERTY(EditAnywhere)
	float MaxOffset = 100.f;

	UPROPERTY(EditAnywhere)
	TArray<UMetahumanComponentsDataAsset*> CrowdAgentMetaHumanPresets;

public:
	UFUNCTION(CallInEditor)
	void GenerateCrowd();

private:
	UFUNCTION(CallInEditor)
	void DestroyCreatedAgents();

	TArray<UMetahumanComponentsDataAsset*> GetRandomizedPoolOfMetaHumanPresets() const;
};
