// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TVRCharacterBase.h"
#include "Character/VRTGripMotionControllerComponent.h"

ATVRCharacterBase::ATVRCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.SetDefaultSubobjectClass<UVRTGripMotionControllerComponent>(AVRBaseCharacter::LeftMotionControllerComponentName)
		.SetDefaultSubobjectClass<UVRTGripMotionControllerComponent>(AVRBaseCharacter::RightMotionControllerComponentName)
	)
{

}

void ATVRCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
