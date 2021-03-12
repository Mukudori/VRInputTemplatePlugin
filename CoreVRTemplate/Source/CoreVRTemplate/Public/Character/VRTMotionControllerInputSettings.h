// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VRTMotionControllerInputSettings.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class COREVRTEMPLATE_API UVRTMotionControllerInputSettings : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	class UVRTGripMotionControllerComponent* Owner;

	UPROPERTY()
	class UInputComponent* InputComponent;

public:
	virtual class UWorld* GetWorld() const override;

	/** Called from  UPlayerMotionControllerComponent */
	void Init();
	void Destroy();

	void EnableInput();
	void DisableInput();
	virtual APlayerController* GetOwningController() const;

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Begin Play"))
	void ReceiveBeginPlay();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "End Play"))
		void ReceiveEndPlay();

public:
	UFUNCTION(BlueprintCallable, Category = "InputSetings")
	UVRTGripMotionControllerComponent* GetMotionController() const;

	UInputComponent* GetInputComponent() const { return InputComponent; }
	
};
