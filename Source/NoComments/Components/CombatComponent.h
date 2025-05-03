// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NoComments/DataStructures/Enums/CharacterCombatState.h"
#include "NoComments/DataStructures/Structs/AttackData.h"
#include "CombatComponent.generated.h"

struct FStunAnimationData;
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

	UPROPERTY(BlueprintAssignable)
	FCombatComponentDelegate OnOwnerFinisherStarted;

	UPROPERTY(BlueprintAssignable)
	FCombatComponentDelegate OnOwnerFinisherFinished;

	UPROPERTY(BlueprintAssignable)
	FCombatComponentDelegate OnOwnerStunFinished;

private:
	UPROPERTY( EditDefaultsOnly, Category="Settings" )
	TSoftObjectPtr<UCombatSettingsDataAsset> CombatSettings = nullptr;

	UPROPERTY()
	AActor* Opponent = nullptr;

	UPROPERTY()
	ECharacterCombatState CharacterCombatState = ECharacterCombatState::NotInCombat;

	// TODO: Probably, should not be here, but given that State Tree can not change it's inner variables, we need to
	UPROPERTY()
	int32 NumberOfAttackTakenBeforeBlock = 0;

	UPROPERTY()
	float CurrentHealth = 0.0f;

	UPROPERTY()
	FAttackData ActiveAttackData = FAttackData::EmptyAttackData;


#pragma region STAMINA
	UPROPERTY()
	float CurrentStamina = 0.0f;

	UPROPERTY()
	FTimerHandle StaminaRestoreTimerHandle;

	UPROPERTY()
	float StaminaRestoreAmount = 1.f;

#pragma endregion

#pragma region STUN
	UPROPERTY()
	UAnimMontage* ActiveStunMontage = nullptr;

	UPROPERTY()
	float ActiveKnockBackPassedTime = 0.f;

	UPROPERTY()
	float ActiveKnockBackTargetTime = 0.f;

#pragma endregion

	UPROPERTY()
	UAnimMontage* ActiveMontage = nullptr;


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
	void Attack(const FAttackData& AttackData);

	// Function to be called by the anim notify state to spawn the damage dealing sphere
	UFUNCTION(BlueprintCallable)
	void SpawnDamageDealingSphereForActiveAttack();

	// Function to be called by the anim notify state to remove the damage dealing sphere
	UFUNCTION(BlueprintCallable)
	void RemoveSpawnedDamageDealingSphere();

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
	void PerformPostStunFinishedActions(UAnimMontage* FinishedStunMontage, bool bInterrupted);

	UFUNCTION()
	void OnOwnerTakeDamage(AActor* DamagedActor,
	                       float Damage,
	                       const class UDamageType* DamageType,
	                       class AController* InstigatedBy,
	                       AActor* DamageCauser);

	FStunAnimationData GetStunAnimationDataForDamage(float Damage) const;

	void AddKnockBackImpulsePerTick();



	bool CanSoftLockOnOpponent() const;

	void SoftLockOnOpponent();

	void UpdateOwnerControlRotationAfterSoftLock();

	void RestoreStamina();

	void KnockOutOwner();

	bool TryBlockAttack(float AttackDamage);

	UFUNCTION()
	void PlayFinisher();

	UFUNCTION()
	void OnFinisherFinished(UAnimMontage* FinishedFinisherMontage, bool bInterrupted);

	void TurnOnFinisherCamera();

	void TurnOffFinisherCamera();

	UCombatComponent* GetOpponentCombatComponent() const;
};
