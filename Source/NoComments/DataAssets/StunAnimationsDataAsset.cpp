// Fill out your copyright notice in the Description page of Project Settings.

#include "StunAnimationsDataAsset.h"

float FStunAnimationData::GetStunAnimationPlayRate() const
{
	if (!IsValid( StunAnimation.LoadSynchronous() ))
	{
		return 1.f;
	}

	return StunDuration / StunAnimation.LoadSynchronous()->GetPlayLength();
}

const TArray<FStunAnimationData>& UStunAnimationsDataAsset::GetStunAnimations() const
{
	return StunAnimations;
}
