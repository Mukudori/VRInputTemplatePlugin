// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VRTGripMotionControllerComponent.h"

void UVRTGripMotionControllerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVRTGripMotionControllerComponent, InteractionComponent);
}

UVRTGripMotionControllerComponent::UVRTGripMotionControllerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UVRTGripMotionControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (InteractionClass)
	{
		bool bInteractionReplicated = InteractionClass->GetDefaultObject<UVRTMotionInteractionComponent>()->GetIsReplicated();

		if (!bInteractionReplicated || GetOwner()->HasAuthority())
		{
			InteractionComponent = NewObject<UVRTMotionInteractionComponent>(this, InteractionClass);
			InteractionComponent->SetRelativeTransform(InteractionTransform);

			InteractionComponent->SetupAttachment(this);
			InteractionComponent->RegisterComponent();
		}
	}

	if (bLateInit)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UVRTGripMotionControllerComponent::ResetInput);
	}
	else
	{
		ResetInput();
	}
}

void UVRTGripMotionControllerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (InputSettings)
	{
		InputSettings->Destroy();
	}
	if (IsValid(InteractionComponent))
	{
		InteractionComponent->DestroyComponent();
	}
	Super::EndPlay(EndPlayReason);
}

void UVRTGripMotionControllerComponent::ResetInput()
{
	if (InputSettings)
	{
		InputSettings->Destroy();
	}

	if (InputSettingsClass && (GetOwningController() && GetOwningController()->IsLocalController()))
	{
		InputSettings = NewObject<UVRTMotionControllerInputSettings>(this, InputSettingsClass);
		if (InputSettings)
		{
			InputSettings->Init();
		}
	}
}

void UVRTGripMotionControllerComponent::SetSettingsClass(TSubclassOf<UVRTMotionControllerInputSettings> NewClass)
{
	InputSettingsClass = NewClass;
	ResetInput();
}

void UVRTGripMotionControllerComponent::PlayHapticEffect(UHapticFeedbackEffect_Base* Effect, float Scale, bool bLoop)
{
	if (Effect && Scale > 0)
	{
		if (APlayerController* PC = GetOwningController())
		{
			EControllerHand Hand;
			GetHandType(Hand);
			PC->PlayHapticEffect(Effect, Hand, Scale, bLoop);
		}
	}
}

void UVRTGripMotionControllerComponent::StopHaptics()
{
	if (APlayerController* PC = GetOwningController())
	{
		EControllerHand Hand;
		GetHandType(Hand);
		PC->StopHapticEffect(Hand);
	}
}

UVRTMotionControllerInputSettings* UVRTGripMotionControllerComponent::GetInputSettings() const
{
	return InputSettings;
}

TSubclassOf<UVRTMotionControllerInputSettings> UVRTGripMotionControllerComponent::GetInputSettingsClass() const
{
	return InputSettingsClass;
}

UVRTMotionInteractionComponent* UVRTGripMotionControllerComponent::GetInteractionComponnet() const
{
	return InteractionComponent;
}

APlayerController* UVRTGripMotionControllerComponent::GetOwningController() const
{
	APlayerController* PC = nullptr;
	if (APawn* OwningPawn = Cast<APawn>(GetOwner()))
	{
		PC = OwningPawn->GetController<APlayerController>();
	}
	return PC;
}
