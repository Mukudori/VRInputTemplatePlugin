// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRCharacter.h"
#include "TVRCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class COREVRTEMPLATE_API ATVRCharacterBase : public AVRCharacter
{
	GENERATED_BODY()
	
		// Grip Motion Controllers Init
	ATVRCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;


	
};
