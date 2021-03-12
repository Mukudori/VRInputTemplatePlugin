// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/W3DUserWidgetBase.h"
#include "W3DUserWidgetKeyboard.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostText, const FString&, TextEnteded);

UCLASS()
class COREVRTEMPLATE_API UW3DUserWidgetKeyboard : public UW3DUserWidgetBase
{
	GENERATED_BODY()



	UFUNCTION(BlueprintCallable)
	void Call_OnTextEntered(const FString& enteredString); 

public:

	UPROPERTY(BlueprintAssignable, Category = "Dispatchers")
	FPostText OnTextEntered; 
	
};
