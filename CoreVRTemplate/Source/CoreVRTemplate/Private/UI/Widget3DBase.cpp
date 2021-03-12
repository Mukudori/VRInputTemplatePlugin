// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget3DBase.h"

// Sets default values
AWidget3DBase::AWidget3DBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it

	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	RootComponent = root;

	widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

	widget->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);

	bNetLoadOnClient = true;
	

}

// Called when the game starts or when spawned
void AWidget3DBase::BeginPlay()
{
	Super::BeginPlay();
	CreateWidgetFromClass(widgetClass); // If faild GetWorld() in Editor
	
}

// Called every frame
void AWidget3DBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWidget3DBase::SetWidget(UUserWidget* NewWidget)
{
	if (NewWidget != nullptr)
		widget->SetWidget(NewWidget);
	else 
		UE_LOG(LogTemp, Error, TEXT("%s: Can't set widget. NewWidget == nullptr."), *GetName());

}

UUserWidget* AWidget3DBase::GetWidget() const
{
	return widget->GetWidget();
}

void AWidget3DBase::CreateWidgetFromClass(TSubclassOf<UUserWidget> NewWidgetClass)
{
	widgetClass = NewWidgetClass;
	if (widgetClass)
	{

		if (widget == nullptr || widget->GetWidgetClass() != widgetClass)
		{
			widget->SetWidget(
				CreateWidget(GetWorld(), widgetClass)
			);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("%s: WidgetClass is already exists"), *GetName());			
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s:WidgetClass is null into CreateWidget"), *GetName());		
	}
}

