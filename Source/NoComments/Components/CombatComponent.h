// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NoComments/DataStructures/Enums/CharacterCombatState.h"
#include "CombatComponent.generated.h"

class UCombatSettingsDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FCombatComponentDelegate );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class NOCOMMENTS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY( BlueprintAssignable )
	FCombatComponentDelegate OnAttackFinished;

private:
	UPROPERTY( EditDefaultsOnly, Category="Settings" )
	TSoftObjectPtr<UCombatSettingsDataAsset> CombatSettings = nullptr;

	UPROPERTY( EditDefaultsOnly, Category="Settings" )
	float DamageDealingSphereComponentRadius = 50.0f;

	UPROPERTY()
	AActor* Opponent = nullptr;

	UPROPERTY()
	ECharacterCombatState CharacterCombatState = ECharacterCombatState::Idle;

public:
#pragma region  FIGHT MODE
	UFUNCTION( BlueprintCallable )
	void ActivateFightMode(AActor* NewOpponent);

	UFUNCTION( BlueprintCallable )
	void DeactivateFightMode();
#pragma endregion

#pragma region BLOCK
	UFUNCTION( BlueprintCallable )
	void ActivateBlock();

	UFUNCTION( BlueprintCallable )
	void DeactivateBlock();
#pragma endregion

	UFUNCTION( BlueprintPure )
	ECharacterCombatState GetCharacterCombatState() const;

	UFUNCTION(BlueprintPure)
	AActor* GetTargetOpponent() const;

protected:
	// Generic function to play any attack montage
	UFUNCTION( BlueprintCallable )
	void PlayAttackMontage(const FName& DamageDealingComponentSocketName, TSoftObjectPtr<UAnimMontage> MontageToPlay);

private:
#pragma region WALK SPEED

	// Helper function for setting the owner's walk speed (with all the included checks)
	void SetOwnerWalkSpeed(float NewWalkSpeed);

	// Helper function for setting the owner's walk speed to the idle walk speed (with all the included checks)
	void SetOwnerWalkSpeed_FightModeDefault();

	// Helper function for setting the owner's walk speed to the block walk speed (with all the included checks)
	void SetOwnerWalkSpeed_FightModeBlock();

#pragma	endregion

	UAnimInstance* GetOwnerAnimInstance() const;

	UFUNCTION()
	void PerformPostAttackFinishedActions(UAnimMontage* FinishedAttackMontage, bool bInterrupted);
};
