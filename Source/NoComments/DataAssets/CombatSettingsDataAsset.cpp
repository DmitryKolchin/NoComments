#include "CombatSettingsDataAsset.h"

float UCombatSettingsDataAsset::GetAttackSpeed() const
{
	return AttackSpeed;
}

float UCombatSettingsDataAsset::GetBlockWalkSpeed() const
{
	return FightModeWalkSpeed_Block;
}

float UCombatSettingsDataAsset::GetIdleWalkSpeed() const
{
	return FightModeWalkSpeed_Default;
}

float UCombatSettingsDataAsset::GetDamageDealingSphereRadius() const
{
	return DamageDealingSphereRadius;
}

bool UCombatSettingsDataAsset::ShouldShowDamageDealingSphere() const
{
	return DEBUG_ShowDamageDealingSphere;
}

float UCombatSettingsDataAsset::GetMaxHealth() const
{
	return MaxHealth;
}

float UCombatSettingsDataAsset::GetMaxStamina() const
{
	return MaxStamina;
}

float UCombatSettingsDataAsset::GetSoftLockDistance() const
{
	return SoftLockDistance;
}

float UCombatSettingsDataAsset::GetStaminaRestoreRate() const
{
	return StaminaRestoreRate;
}

float UCombatSettingsDataAsset::GetStaminaAfterAttackRestoreDelay() const
{
	return StaminaAfterAttackRestoreDelay;
}

float UCombatSettingsDataAsset::GetStaminaAfterBlockRestoreDelay() const
{
	return StaminaAfterBlockRestoreDelay;
}

TSoftObjectPtr<UAnimMontage> UCombatSettingsDataAsset::GetKnockoutAnimMontage() const
{
	return KnockoutAnimMontage;
}

UStunAnimationsDataAsset* UCombatSettingsDataAsset::GetStunAnimationsDataAsset() const
{
	return StunAnimationsDataAsset;
}

UFinisherAnimationsDataAsset* UCombatSettingsDataAsset::GetFinishersAnimationsDataAsset() const
{
	return FinishersAnimationsDataAsset;
}
