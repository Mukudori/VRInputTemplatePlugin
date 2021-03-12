// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "W3DInterface.h"
#include "Widget3DBase.generated.h"

UCLASS()
class COREVRTEMPLATE_API AWidget3DBase : public AActor, public IW3DInterface // For implemment interface all childs
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	USceneComponent* root;
	
	UPROPERTY()
	UWidgetComponent* widget;

	TSubclassOf<UUserWidget> widgetClass; 
	
public:	
	// Sets default values for this actor's properties
	AWidget3DBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWidget(UUserWidget* NewWidget);

	UFUNCTION(BlueprintCallable)
	virtual UUserWidget* GetWidget() const;


	UFUNCTION(BlueprintCallable)
	void CreateWidgetFromClass(TSubclassOf<UUserWidget> NewWidgetClass);


};
