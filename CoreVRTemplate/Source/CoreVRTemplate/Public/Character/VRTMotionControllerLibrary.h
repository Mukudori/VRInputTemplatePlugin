// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VRTMotionControllerInputSettings.h"
#include "Components/InputComponent.h"
#include "Misc/RuntimeErrors.h"
#include "XRMotionControllerBase.h"

#include "VRTMotionControllerLibrary.generated.h"


DECLARE_DYNAMIC_DELEGATE(FSimpleDynamicDelegate);



UENUM()
enum class EHandSlot : uint8
{
	Any,
	Left,
	Right
};

UENUM()
enum class EBranchMatchingResult : uint8
{
	HandMatch,
	HandMismatch
};

UCLASS(meta = (RestrictedToClasses = "VRTMotionControllerInputSettings"))
class COREVRTEMPLATE_API UVRTMotionControllerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Player|Input", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
	static void GetControllerHand(UVRTMotionControllerInputSettings* NodeOwner, EControllerHand& Hand);


	UFUNCTION(BlueprintPure, Category = "Player|Input", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
	static void GetHandType(UVRTMotionControllerInputSettings* NodeOwner, EHandSlot& Hand);

	UFUNCTION(BlueprintCallable, Category = "Player|Input", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner", ExpandEnumAsExecs = "Hand"))
	static void SwitchHandType(UVRTMotionControllerInputSettings* NodeOwner, EHandSlot& Hand);

	UFUNCTION(BlueprintCallable, Category = "Player|Input", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner", ExpandEnumAsExecs = "Result"))
	static void BranchHandCheck(UVRTMotionControllerInputSettings* NodeOwner, EHandSlot RequiredHand, EBranchMatchingResult& Result);


	/** Bind delegate to key action matching controller hand */
	UFUNCTION(BlueprintCallable, Category = "Player|Input", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
	static void BindActionDelegate(UVRTMotionControllerInputSettings* NodeOwner, FName LeftHandAction, FName RightHandAction, FInputActionHandlerDynamicSignature Press, FInputActionHandlerDynamicSignature Release);

	/** Bind delegate to key action matching controller hand */
	UFUNCTION(BlueprintCallable, Category = "Player|Input", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
	static void BindAxisDelegate(UVRTMotionControllerInputSettings* NodeOwner, FName LeftHandAxis, FName RightHandAxis, FInputAxisHandlerDynamicSignature Value);
};
