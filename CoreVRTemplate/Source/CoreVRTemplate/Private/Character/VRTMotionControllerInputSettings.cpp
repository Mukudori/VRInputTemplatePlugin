// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VRTMotionControllerInputSettings.h"
#include "Character/VRTGripMotionControllerComponent.h"
#include "GameFramework/Pawn.h"

UWorld* UVRTMotionControllerInputSettings::GetWorld() const
{
	if (GetOuter()->IsA<UVRTGripMotionControllerComponent>())
	{
		return GetOuter()->GetWorld();
	}

	return nullptr;
}

void UVRTMotionControllerInputSettings::Init()
{
	Owner = Cast<UVRTGripMotionControllerComponent>(GetOuter());
	if (Owner == nullptr)
	{
		MarkPendingKill();
		return;
	}

	EnableInput();
	if (GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint) || !GetClass()->HasAnyClassFlags(CLASS_Native))
	{
		ReceiveBeginPlay();
	}
}

void UVRTMotionControllerInputSettings::Destroy()
{
	DisableInput();
	if (!HasAnyFlags(RF_BeginDestroyed) && !IsUnreachable() && (GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint) || !GetClass()->HasAnyClassFlags(CLASS_Native)))
	{
		ReceiveEndPlay();
	}

	MarkPendingKill();
}

void UVRTMotionControllerInputSettings::EnableInput()
{
	APlayerController* PC = GetOwningController();

	if (PC && PC->IsLocalController())
	{
		// If it doesn't exist create it and bind delegates
		if (!InputComponent)
		{
			FName Name = *(GetName() + TEXT("_InputComponent"));
			InputComponent = NewObject<UInputComponent>(this, Name);
			InputComponent->bBlockInput = false;
			InputComponent->Priority = 5;
		}

		PC->PushInputComponent(InputComponent);
	}
}

void UVRTMotionControllerInputSettings::DisableInput()
{
	APlayerController* PC = GetOwningController();
	if (PC)
	{
		if (InputComponent)
		{
			PC->PopInputComponent(InputComponent);
			InputComponent->DestroyComponent();
			InputComponent = nullptr;
		}
	}
}

APlayerController* UVRTMotionControllerInputSettings::GetOwningController() const
{
	return Owner->GetOwningController();
}

UVRTGripMotionControllerComponent* UVRTMotionControllerInputSettings::GetMotionController() const
{
	return Owner;
}
