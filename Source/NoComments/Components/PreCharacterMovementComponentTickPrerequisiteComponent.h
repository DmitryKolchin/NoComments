// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PreCharacterMovementComponentTickPrerequisiteComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOCOMMENTS_API UPreCharacterMovementComponentTickPrerequisiteComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPreCharacterMovementComponentTickPrerequisiteComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY( EditDefaultsOnly, Category="Strafe" )
	TSoftObjectPtr<UCurveFloat> StrafeSpeedMapCurve;

	UPROPERTY( EditDefaultsOnly, Category="Walking" )
	FVector WalkSpeeds = FVector{200.f, 180.f, 150.f};

	UPROPERTY( EditDefaultsOnly, Category="Crouching" )
	FVector CrouchSpeeds = FVector{200.f, 180.f, 150.f};

private:
	void UpdateOwnerCharacterMovement();

	double CalculateMaxSpeedFromDirection(UCharacterMovementComponent* OwnerCharacterMovement,
	                                      const FVector& Speeds,
	                                      const FRotator& OwnerControlRotation) const;
};
