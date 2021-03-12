// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Keyboard/W3DKeyboard.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"


void AW3DKeyboard::Tick(float DeltaTime)
{
	if (widget == nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Widget3D ERROR: WidgetClass is null"));
	
	Super::Tick(DeltaTime);
}

void AW3DKeyboard::BeginPlay()
{
	Super::BeginPlay();
	
}

AW3DKeyboard::AW3DKeyboard()
{	
	static ConstructorHelpers::FClassFinder<UW3DUserWidgetKeyboard> ClassFinder(TEXT("WidgetBlueprint'/CoreVRTemplate/UI/KeyboardWidget_BP.KeyboardWidget_BP_C'"));
	CreateWidgetFromClass(ClassFinder.Class); 

	PrimaryActorTick.bCanEverTick = true;
}
