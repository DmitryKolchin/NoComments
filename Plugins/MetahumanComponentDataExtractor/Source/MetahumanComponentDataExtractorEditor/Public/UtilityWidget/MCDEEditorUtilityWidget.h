// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "MCDEEditorUtilityWidget.generated.h"

/**
 * 
 */
UCLASS()
class METAHUMANCOMPONENTDATAEXTRACTOREDITOR_API UMCDEEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowedClasses="Blueprint"))
	UBlueprint* MetahumanBlueprintProperty;
};
