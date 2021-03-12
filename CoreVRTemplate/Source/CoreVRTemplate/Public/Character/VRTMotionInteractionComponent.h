// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GripMotionControllerComponent.h"
#include "VRTMotionInteractionComponent.generated.h"


UENUM()
enum class EGripCondition : uint8
{
	Emply,
	CouldGrip,
	Gripped
};



UCLASS(Abstract, Blueprintable, Category = "Character")
class COREVRTEMPLATE_API UVRTMotionInteractionComponent : public USphereComponent
{
	GENERATED_BODY()
	
	UPROPERTY()
	UGripMotionControllerComponent* GripController;

public:
	/** Same as with actor. Disabled by default because component is attached to already replicated motion controller */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "ComponentReplication")
		bool bReplicateMovement;



public:

	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	UGripMotionControllerComponent* GetGripController() const;

	/** Get overlapped components that implements VRGripInterface */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void GetGrippableComponents(TArray<UPrimitiveComponent*>& OutGrippableComponents, bool bCheckActorInterface = true) const;

	/** Get overlapped actors that implements VRGripInterface */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void GetGrippableActors(TArray<AActor*>& OutGrippableActors, TSubclassOf<AActor> ClassFilter) const;

	/**
	 * Check if VRGripInterface object is in range
	 * @param bOnlyAllowedGrips additionally check DenyGripping(actor has priority over component)
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool IsGrippableInRange(bool bOnlyAllowedGrips = true) const;

	/** Find closest */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* GetClosestGrippableActor(TSubclassOf<AActor> ClassFilter);

	/** Find closest */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	UPrimitiveComponent* GetClosestGrippableComponent(bool bCheckActorInterface = true);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	EGripCondition GetGripState() const;
};
