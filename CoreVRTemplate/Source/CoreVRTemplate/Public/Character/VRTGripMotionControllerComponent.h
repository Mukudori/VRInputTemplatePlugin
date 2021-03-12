// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GripMotionControllerComponent.h"
#include "VRTMotionControllerInputSettings.h"
#include "VRTMotionInteractionComponent.h"
#include "VRTGripMotionControllerComponent.generated.h"

/**
 * 
 */
UCLASS()
class COREVRTEMPLATE_API UVRTGripMotionControllerComponent : public UGripMotionControllerComponent
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess))
	TSubclassOf<UVRTMotionControllerInputSettings> InputSettingsClass;

	UPROPERTY()
	UVRTMotionControllerInputSettings* InputSettings;

	/** Initialize on next tick after BeginPlay. */
	UPROPERTY(EditAnywhere, Category = "Input")
	bool bLateInit;


	/** Gripping logic and actions */
	UPROPERTY(EditAnywhere, Category = "Grip", meta = (AllowPrivateAccess))
	TSubclassOf<UVRTMotionInteractionComponent> InteractionClass;

	UPROPERTY(EditAnywhere, Category = "Grip", meta = (AllowPrivateAccess))
	FTransform InteractionTransform;

	UPROPERTY(Replicated)
	class UVRTMotionInteractionComponent* InteractionComponent;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	UVRTGripMotionControllerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Reinitializes input */
	UFUNCTION(BlueprintCallable, Category = "Player|Input")
	void ResetInput();

	UFUNCTION(BlueprintCallable, Category = "Player|Input")
	void SetSettingsClass(TSubclassOf<UVRTMotionControllerInputSettings> NewClass);



	UFUNCTION(BlueprintCallable, Category = "Player")
	void PlayHapticEffect(UHapticFeedbackEffect_Base* Effect, float Scale = 1.0f, bool bLoop = false);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopHaptics();


	// Accessors

	UFUNCTION(BlueprintCallable, Category = "Player|Input")
	UVRTMotionControllerInputSettings* GetInputSettings() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Input")
	TSubclassOf<UVRTMotionControllerInputSettings> GetInputSettingsClass() const;

	UFUNCTION(BlueprintCallable, Category = "Player")
	UVRTMotionInteractionComponent* GetInteractionComponnet() const;

	UFUNCTION(BlueprintCallable, Category = "Player")
	APlayerController* GetOwningController() const;
	
};
