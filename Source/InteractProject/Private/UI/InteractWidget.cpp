// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractWidget.h"
#include "Component/InteractComponent.h"


void UInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const auto Player = GetOwningPlayerPawn();
	
	if (!Player) return;
	
	UInteractComponent* InteractComponent = Cast<UInteractComponent>(Player->GetComponentByClass(UInteractComponent::StaticClass()));

	if (InteractComponent)
	{
		InteractComponent->OnUpdateInteractWidget.AddUObject(this, &UInteractWidget::SetVisibility);
	}
}
