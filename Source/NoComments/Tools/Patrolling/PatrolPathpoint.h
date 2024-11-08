// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPathpoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FPatrolPathpointDelegate, int32, PathpointIndex );

UCLASS()
class NOCOMMENTS_API APatrolPathpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPathpoint();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* Billboard;

	// Flag to check if the object was destroyed
	UPROPERTY()
	bool bWasDestroyed = false;

	UPROPERTY()
	int32 PathpointIndex = 0;

public:
	void SetPathpointIndex( int32 NewPathpointIndex );

	int32 GetPathpointIndex() const { return PathpointIndex; }

public:
	FPatrolPathpointDelegate OnAddForwardPathpointRequested;

	FPatrolPathpointDelegate OnAddBackPathpointRequested;

protected:
	UFUNCTION(BlueprintImplementableEvent, CallInEditor)
	void UpdatePathpointIndexText( int32 NewIndex );

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Actions")
	void AddPathpointForward();

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Actions")
	void AddPathpointBackward();


};
