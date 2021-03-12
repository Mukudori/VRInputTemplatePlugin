// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VRTMotionInteractionComponent.h"
#include "VRGripInterface.h"
#include <Algo/MinElement.h>

void UVRTMotionInteractionComponent::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	/*
	Relative Loc/Rot/Scale3D of USceneComponent is now private so we cannot set isReplicated this way
	Find a way to do it or fully reject this
	*/
	/*PRAGMA_DISABLE_DEPRECATION_WARNINGS
	DOREPLIFETIME_ACTIVE_OVERRIDE(USceneComponent, RelativeLocation, bReplicateMovement);
	DOREPLIFETIME_ACTIVE_OVERRIDE(USceneComponent, RelativeRotation, bReplicateMovement);
	DOREPLIFETIME_ACTIVE_OVERRIDE(USceneComponent, RelativeScale3D, bReplicateMovement);
	PRAGMA_ENABLE_DEPRECATION_WARNINGS*/
}

void UVRTMotionInteractionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVRTMotionInteractionComponent, bReplicateMovement);
	//DOREPLIFETIME(UMotionControllerInteractionComponent, GrippedObject);
}

void UVRTMotionInteractionComponent::BeginPlay()
{
	GripController = Cast<UGripMotionControllerComponent>(GetAttachParent());
	Super::BeginPlay();

	if (GripController == nullptr)
	{
		GripController = Cast<UGripMotionControllerComponent>(GetAttachParent());
		if (GripController == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("%s: Grip controller not found. Destroying"), *GetName());
			DestroyComponent(true);
		}
	}
}

UGripMotionControllerComponent* UVRTMotionInteractionComponent::GetGripController() const
{
	return GripController;;
}

void UVRTMotionInteractionComponent::GetGrippableComponents(TArray<UPrimitiveComponent*>& OutGrippableComponents, bool bCheckActorInterface) const
{
	//Same as GetOverlappedComponents but with interface check

	if (OverlappingComponents.Num() <= 12)
	{
		// TArray with fewer elements is faster than using a set (and having to allocate it).
		OutGrippableComponents.Reset(OverlappingComponents.Num());
		for (const FOverlapInfo& OtherOverlap : OverlappingComponents)
		{
			if (UPrimitiveComponent* const OtherComp = OtherOverlap.OverlapInfo.Component.Get())
			{
				if (OtherComp->Implements<UVRGripInterface>() || (bCheckActorInterface && OtherComp->GetOwner() && OtherComp->GetOwner()->Implements<UVRGripInterface>()))
				{
					OutGrippableComponents.AddUnique(OtherComp);
				}
			}
		}
	}
	else
	{
		// Fill set (unique)
		TSet<UPrimitiveComponent*> OverlapSet;
		GetOverlappingComponents(OverlapSet);

		// Copy to array
		OutGrippableComponents.Reset(OverlapSet.Num());
		for (UPrimitiveComponent* OtherComp : OverlapSet)
		{
			if (OtherComp->Implements<UVRGripInterface>() || (bCheckActorInterface && OtherComp->GetOwner() && OtherComp->GetOwner()->Implements<UVRGripInterface>()))
			{
				OutGrippableComponents.Add(OtherComp);
			}
		}
	}
}

void UVRTMotionInteractionComponent::GetGrippableActors(TArray<AActor*>& OutGrippableActors, TSubclassOf<AActor> ClassFilter) const
{
	//Same as GetOverlappedComponents but with interface check

	if (OverlappingComponents.Num() <= 12)
	{
		// TArray with fewer elements is faster than using a set (and having to allocate it).
		OutGrippableActors.Reset(OverlappingComponents.Num());
		for (const FOverlapInfo& OtherOverlap : OverlappingComponents)
		{
			if (UPrimitiveComponent* OtherComponent = OtherOverlap.OverlapInfo.Component.Get())
			{
				AActor* OtherActor = OtherComponent->GetOwner();
				if (OtherActor && OtherActor->Implements<UVRGripInterface>() && ((*ClassFilter) == nullptr || OtherActor->IsA(ClassFilter)))
				{
					OutGrippableActors.AddUnique(OtherActor);
				}
			}
		}
	}
	else
	{
		// Fill set (unique)
		TSet<AActor*> OverlapSet;
		GetOverlappingActors(OverlapSet, ClassFilter);

		// Copy to array
		OutGrippableActors.Reset(OverlapSet.Num());
		for (AActor* OverlappingActor : OverlapSet)
		{
			if (OverlappingActor->Implements<UVRGripInterface>())
			{
				OutGrippableActors.Add(OverlappingActor);
			}
		}
	}
}

bool UVRTMotionInteractionComponent::IsGrippableInRange(bool bOnlyAllowedGrips) const
{
	for (const FOverlapInfo& OtherOverlap : OverlappingComponents)
	{
		if (UPrimitiveComponent* const OtherComp = OtherOverlap.OverlapInfo.Component.Get())
		{
			if (bOnlyAllowedGrips)
			{
				UObject* VRGrippable = nullptr;
				VRGrippable = OtherComp->Implements<UVRGripInterface>() ? OtherComp : VRGrippable;
				VRGrippable = (OtherComp->GetOwner() && OtherComp->GetOwner()->Implements<UVRGripInterface>()) ? OtherComp->GetOwner() : VRGrippable;

				if (VRGrippable && !IVRGripInterface::Execute_DenyGripping(VRGrippable))
				{
					return true;
				}
			}
			else if (OtherComp->Implements<UVRGripInterface>() || (OtherComp->GetOwner() && OtherComp->GetOwner()->Implements<UVRGripInterface>()))
			{
				return true;
			}
		}
	}

	return false;
}

AActor* UVRTMotionInteractionComponent::GetClosestGrippableActor(TSubclassOf<AActor> ClassFilter)
{
	TArray<AActor*> GrippableActors;
	GetGrippableActors(GrippableActors, ClassFilter);

	FVector Location = GetComponentLocation();
	AActor** Closest = Algo::MinElementBy(GrippableActors, [&Location](AActor* Actor) { return FVector::DistSquared(Actor->GetActorLocation(), Location); });
	return (Closest != nullptr) ? *Closest : nullptr;
}

UPrimitiveComponent* UVRTMotionInteractionComponent::GetClosestGrippableComponent(bool bCheckActorInterface)
{
	TArray<UPrimitiveComponent*> GrippableComponents;
	GetGrippableComponents(GrippableComponents, bCheckActorInterface);

	FVector Location = GetComponentLocation();
	UPrimitiveComponent** Closest = Algo::MinElementBy(GrippableComponents, [&Location](UPrimitiveComponent* Comp) { return FVector::DistSquared(Comp->GetComponentLocation(), Location); });
	return (Closest != nullptr) ? *Closest : nullptr;
}

EGripCondition UVRTMotionInteractionComponent::GetGripState_Implementation() const
{
	return EGripCondition::Emply;
}

void UVRTMotionInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
