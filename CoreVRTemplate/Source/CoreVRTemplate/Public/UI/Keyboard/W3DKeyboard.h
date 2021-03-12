// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget3DBase.h"
#include "W3DUserWidgetKeyboard.h"
#include "W3DKeyboard.generated.h"

/**
 * 
 */


UCLASS()
class COREVRTEMPLATE_API AW3DKeyboard : public AWidget3DBase
{
	GENERATED_BODY()

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

public:

	AW3DKeyboard(); 
	
};
