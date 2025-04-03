// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NoComments/DataStructures/Enums/CharacterCombatState.h"
#include "CombatComponent.generated.h"

struct FAttackData;
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

	UPROPERTY(BlueprintAssignable)
	FCombatComponentDelegate OnOwnerKnockedOut;

private:
	UPROPERTY( EditDefaultsOnly, Category="Settings" )
	TSoftObjectPtr<UCombatSettingsDataAsset> CombatSettings = nullptr;

	UPROPERTY()
	AActor* Opponent = nullptr;

	UPROPERTY()
	ECharacterCombatState CharacterCombatState = ECharacterCombatState::Idle;

	UPROPERTY()
	int32 NumberOfAttackTakenBeforeBlock = 0;

	UPROPERTY()
	float CurrentHealth = 0.0f;

#pragma region STAMINA
	UPROPERTY()
	float CurrentStamina = 0.0f;

	UPROPERTY()
	FTimerHandle StaminaRestoreTimerHandle;

	UPROPERTY()
	float StaminaRestoreAmount = 1.f;

#pragma endregion

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
	UPARAM( DisplayName = "Character Combat State" )ECharacterCombatState GetCharacterCombatState() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Target Opponent" )AActor* GetTargetOpponent() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Number of Attacks Taken Before Block" )int32 GetNumberOfAttackTakenBeforeBlock() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Current Health" )float GetCurrentHealth() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Current Stamina" )float GetCurrentStamina() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Can Perform Any Attack" ) bool CanPerformAnyAttack() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Can Perform Given Attack" ) bool CanPerformGivenAttack(const FAttackData& AttackData) const;

	// Generic function to play any attack montage
	UFUNCTION( BlueprintCallable )
	void PlayAttackMontage(const FAttackData& AttackData);

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

	UFUNCTION()
	void OnOwnerTakeDamage(AActor* DamagedActor,
	                       float Damage,
	                       const class UDamageType* DamageType,
	                       class AController* InstigatedBy,
	                       AActor* DamageCauser);

	void SpawnDamageDealingSphereForAttack(const FAttackData& AttackData);

	bool CanSoftLockOnOpponent() const;

	void SoftLockOnOpponent();

	void RestoreStamina();

	void KnockOutOwner();
};
