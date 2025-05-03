// Fill out your copyright notice in the Description page of Project Settings.

#include "FinisherAnimationsDataAsset.h"

const TArray<FFinisherInfo>& UFinisherAnimationsDataAsset::GetFinisherAnimations() const
{
	return FinisherAnimations;
}

float UFinisherAnimationsDataAsset::GetDistanceBetweenFighters() const
{
	return DistanceBetweenFighters;
}

FFinisherInfo UFinisherAnimationsDataAsset::GetRandomFinisher() const
{
	if ( FinisherAnimations.Num() == 0 )
	{
		return FFinisherInfo{};
	}

	int32 RandomIndex = FMath::RandRange( 0, FinisherAnimations.Num() - 1 );
	return FinisherAnimations[ RandomIndex ];
}
