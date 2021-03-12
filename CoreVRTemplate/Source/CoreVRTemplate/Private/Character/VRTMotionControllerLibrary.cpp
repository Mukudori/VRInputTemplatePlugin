// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VRTMotionControllerLibrary.h"
#include "Character/VRTGripMotionControllerComponent.h"




void UVRTMotionControllerLibrary::GetControllerHand(UVRTMotionControllerInputSettings* NodeOwner, EControllerHand& Hand)
{
	Hand = EControllerHand::AnyHand;
	if (ensureAsRuntimeWarning(NodeOwner != nullptr))
	{
		UVRTGripMotionControllerComponent* MotionController = NodeOwner->GetMotionController();
		FXRMotionControllerBase::GetHandEnumForSourceName(MotionController->MotionSource, Hand);
	}
}


void UVRTMotionControllerLibrary::GetHandType(UVRTMotionControllerInputSettings* NodeOwner, EHandSlot& Hand)
{
	Hand = EHandSlot::Any;
	if (ensureAsRuntimeWarning(NodeOwner != nullptr))
	{
		UVRTGripMotionControllerComponent* MotionController = NodeOwner->GetMotionController();
		FName MotionSource = MotionController->MotionSource;

		if (MotionSource == FXRMotionControllerBase::LeftHandSourceId || MotionSource == FName("EControllerHand::Left"))
		{
			Hand = EHandSlot::Left;
		}
		else if (MotionSource == FXRMotionControllerBase::RightHandSourceId || MotionSource == FName("EControllerHand::Right"))
		{
			Hand = EHandSlot::Right;
		}
	}
}

void UVRTMotionControllerLibrary::SwitchHandType(UVRTMotionControllerInputSettings* NodeOwner, EHandSlot& Hand)
{
	GetHandType(NodeOwner, Hand);
}

void UVRTMotionControllerLibrary::BranchHandCheck(UVRTMotionControllerInputSettings* NodeOwner, EHandSlot RequiredHand, EBranchMatchingResult& Result)
{
	if (RequiredHand == EHandSlot::Any)
	{
		Result = EBranchMatchingResult::HandMatch;
	}
	else
	{
		EHandSlot ControllerHand;
		GetHandType(NodeOwner, ControllerHand);

		Result = (ControllerHand == RequiredHand) ? EBranchMatchingResult::HandMatch : EBranchMatchingResult::HandMismatch;
	}
}

void UVRTMotionControllerLibrary::BindActionDelegate(UVRTMotionControllerInputSettings* NodeOwner, FName LeftHandAction, FName RightHandAction, FInputActionHandlerDynamicSignature Press, FInputActionHandlerDynamicSignature Release)
{
	if (ensureAsRuntimeWarning(NodeOwner != nullptr))
	{
		if (UInputComponent* InputComponent = NodeOwner->GetInputComponent())
		{
			EHandSlot Hand;
			GetHandType(NodeOwner, Hand);

			FName Action = NAME_None;
			if (Hand == EHandSlot::Left)
			{
				Action = LeftHandAction;
			}
			else if (Hand == EHandSlot::Right)
			{
				Action = RightHandAction;
			}
			FInputActionBinding PressAction(Action, EInputEvent::IE_Pressed);
			PressAction.ActionDelegate = FInputActionUnifiedDelegate(Press);

			FInputActionBinding ReleaseAction(Action, EInputEvent::IE_Released);
			ReleaseAction.ActionDelegate = FInputActionUnifiedDelegate(Release);

			InputComponent->AddActionBinding(PressAction);
			InputComponent->AddActionBinding(ReleaseAction);
		}
	}
}

void UVRTMotionControllerLibrary::BindAxisDelegate(UVRTMotionControllerInputSettings* NodeOwner, FName LeftHandAxis, FName RightHandAxis, FInputAxisHandlerDynamicSignature Value)
{
	if (ensureAsRuntimeWarning(NodeOwner != nullptr))
	{
		if (UInputComponent* InputComponent = NodeOwner->GetInputComponent())
		{
			EHandSlot Hand;
			GetHandType(NodeOwner, Hand);

			FName Axis = NAME_None;
			if (Hand == EHandSlot::Left)
			{
				Axis = LeftHandAxis;
			}
			else if (Hand == EHandSlot::Right)
			{
				Axis = RightHandAxis;
			}
			InputComponent->BindAxis(Axis).AxisDelegate.BindDelegate(Value.GetUObject(), Value.GetFunctionName());
		}
	}
}
