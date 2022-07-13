// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InteractComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "InteractProject/InteractProjectCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	PlayerCharacter = Cast<AInteractProjectCharacter>(GetOwner());

	if (!PlayerCharacter || !World) return;

	PlayerCamera = PlayerCharacter->GetCharacterCamera();
	World->GetTimerManager().SetTimer(InteractTraceTimerHandle, this, &UInteractComponent::InteractTrace, 0.05f, true);
}

void UInteractComponent::InteractTrace()
{
	if (!PlayerCharacter || !PlayerCamera || bPauseInteractTrace) return;

	FHitResult Result;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(PlayerCharacter);
	ESlateVisibility Visibility;
	const FVector TraceStart = PlayerCamera->GetComponentLocation() + (PlayerCamera->GetForwardVector());
	const FVector TraceEnd = TraceStart + (PlayerCamera->GetForwardVector() * InteractDistance);
	
	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceStart, TraceEnd, 5.0f, TraceTypeQuery3,
															  false, IgnoreActors, EDrawDebugTrace::None, Result, true);
	
	if (bHit && Result.Component->ComponentHasTag("Interact"))
	{
		ActorInFocus = Result.GetActor();
		ActiveComponent = Result.GetComponent();
		Visibility = ESlateVisibility::SelfHitTestInvisible;
	}
	else
	{
		ActorInFocus = nullptr;
		ActiveComponent = nullptr;
		Visibility = ESlateVisibility::Collapsed;
	}
	OnUpdateInteractWidget.Broadcast(Visibility);
}

void UInteractComponent::StartInteract()
{
	if (!ActorInFocus || bInspectorActivated) return;
	
	if (ActorInFocus->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		if (PlayerCharacter && ActiveComponent && ActiveComponent->ComponentHasTag("Item"))
		{
			bInspectorActivated = true;
			PlayerCharacter->DisableMovement();
			bPauseInteractTrace = true;
		}
		Cast<IInteractInterface>(ActorInFocus)->Interact(this);
	}
}

FVector UInteractComponent::GetCamLocationInInspectMode() const
{
	if (!PlayerCamera)
	{
		return FVector::ZeroVector;
	}
	return PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * InspectDistance;
}

FRotator UInteractComponent::GetCamRotationInInspectMode() const
{
	if (!PlayerCamera)
	{
		return FRotator::ZeroRotator;
	}
	return PlayerCamera->GetComponentRotation();
}

UPrimitiveComponent* UInteractComponent::GetActiveComponent() const
{
	return ActiveComponent;
}

void UInteractComponent::SetInteractTracePause(const bool bPaused)
{
	bPauseInteractTrace = bPaused;
}

void UInteractComponent::ToggleInspectMode(const bool bActivated)
{
	bInspectorActivated = bActivated;
}