// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestCharacter.generated.h"

class UMetahumanBuilderComponent;

UCLASS(Blueprintable)
class NOCOMMENTS_API ATestCharacter : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	UMetahumanBuilderComponent* MetahumanBuilderComponent;

};
