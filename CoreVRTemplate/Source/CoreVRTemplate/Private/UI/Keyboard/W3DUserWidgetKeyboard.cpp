// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Keyboard/W3DUserWidgetKeyboard.h"

void UW3DUserWidgetKeyboard::Call_OnTextEntered(const FString& enteredString)
{
	OnTextEntered.Broadcast(enteredString); 
}
