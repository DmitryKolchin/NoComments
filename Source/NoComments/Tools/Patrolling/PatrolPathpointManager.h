// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PatrolPathpoint.h"
#include "GameFramework/Actor.h"
#include "PatrolPathpointManager.generated.h"

UENUM(BlueprintType)
enum class EPatrolPathType : uint8
{
	BackAndForth,
	Loop
};

USTRUCT(BlueprintType)
struct FPatrolPathData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	EPatrolPathType PathType = EPatrolPathType::BackAndForth;

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> PathpointLocations;


};


UCLASS()
class NOCOMMENTS_API APatrolPathpointManager : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	APatrolPathpointManager();

	virtual bool ShouldTickIfViewportsOnly() const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;

	virtual void PostLoad() override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;


private:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UBillboardComponent* Billboard;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APatrolPathpoint> PathpointClass = APatrolPathpoint::StaticClass();

	UPROPERTY()
	TArray<APatrolPathpoint*> Pathpoints;

	UPROPERTY(EditAnywhere, Category="Path Settings")
	EPatrolPathType PathType = EPatrolPathType::BackAndForth;

	UPROPERTY(EditAnywhere, Category="Lines Settings")
	FColor LinesColor = FColor::Red;

	UPROPERTY(EditAnywhere, Category="Lines Settings")
	float LinesThickness = 2.f;


public:
	UFUNCTION(BlueprintPure)
	FPatrolPathData GetPatrolPathData() const;


protected:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Actions")
	void AddPathpointToEnd();

	UFUNCTION(Blueprintable, CallInEditor, Category="Actions")
	void AddPathpointToStart();


private:
	void AddPathpoint(const FVector &NewPathpointLocation, const int32 NewPathpointIndex = INDEX_NONE);

	void RecalculatePathpointsIndexesStartingFromIndex(int32 StartingIndex = 0);

	void RecalculateAllPathpointsIndexes();

	UFUNCTION(CallInEditor)
	void OnAddForwardPathpointRequested(int32 PathpointIndex);

	UFUNCTION(CallInEditor)
	void OnAddBackPathpointRequested(int32 PathpointIndex);

	UFUNCTION(CallInEditor)
	void OnPathpointDestroyed(int32 PathpointIndex);

	void TryBindPathpointEvents(APatrolPathpoint* Pathpoint);




};
