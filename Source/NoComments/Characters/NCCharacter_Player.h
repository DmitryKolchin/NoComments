// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "InputAction.h"
#include "NCCharacter_Base.h"
#include "NoComments/DataAssets/CameraSettingsDataAsset.h"
#include "NCCharacter_Player.generated.h"

class USpringArmComponent;
struct FInputActionValue;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FCameraSettings;
class UCameraSettingsDataAsset;
class UCombatComponent;

UCLASS()
class NOCOMMENTS_API ANCCharacter_Player : public ANCCharacter_Base, public IGenericTeamAgentInterface
{
	GENERATED_BODY()


public:
	ANCCharacter_Player();
	// IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	// ~IGenericTeamAgentInterface

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCombatComponent* CombatComponent;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY()
	FGenericTeamId TeamId;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	UCameraSettingsDataAsset* CameraSettingsDataAsset;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	float CameraTurnRate = 1.f;

	UPROPERTY()
	FCameraSettings ActiveCameraSettings;

	UPROPERTY()
	float CurrentCameraPitchOffset = 0.f;

	UPROPERTY()
	float CurrentCameraYawOffset = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	const UInputMappingContext* DefaultInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> RotateCameraInputAction;



public:
	UFUNCTION(BlueprintPure)
	UCombatComponent* GetCombatComponent() const;

protected:
	UFUNCTION(BlueprintPure)
	FCameraSettings GetActiveCameraSettings() const;

	UFUNCTION(BlueprintCallable)
	void SetFightModeCameraSettings();

	UFUNCTION(BlueprintCallable)
	void ResetCameraSettingsToDefault();

private:
	UFUNCTION()
	void Move(const FInputActionInstance& Value);

	UFUNCTION()
	void RotateCamera(const FInputActionInstance& Value);

};
