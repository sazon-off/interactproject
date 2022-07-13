// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameHUD.h"

#include "Blueprint/UserWidget.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	ensure(InteractWidgetClass);
	
	UWorld* World = GetWorld();

	if (!World) return;
	
	UUserWidget* InteractWidget = CreateWidget<UUserWidget>(World, InteractWidgetClass);
	
	if (!InteractWidget) return;
	
	InteractWidget->AddToViewport();
}
