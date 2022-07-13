// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActors/InteractiveDrawer.h"

#include "Component/InteractComponent.h"
#include "Components/TimelineComponent.h"

void AInteractiveDrawer::BeginPlay()
{
	Super::BeginPlay();
	InitTimeline(TimelineCurveFloat);
}

void AInteractiveDrawer::Interact(UInteractComponent* InteractComponent_In)
{
	InteractComponent = InteractComponent_In;
	ActiveComponent = InteractComponent->GetActiveComponent();
	
	if (bMoving || !InteractComponent || !ActiveComponent) return;

	if (bLocked)
	{
		TryToUnlock();
	}
	else
	{
		InteractComponent->SetInteractTracePause(true);
		bMoving = true;
		OriginRelativeLocation = ActiveComponent->GetRelativeLocation();
		if (PulloutLenght - OriginRelativeLocation.Y > OriginRelativeLocation.Y)
		{
			CurrentPulloutLenght = PulloutLenght;
			PlayInteractSound(OpenSound);
		}
		else
		{
			CurrentPulloutLenght = 0.0f;
			PlayInteractSound(CloseSound);
		}

		TimelineComponent->PlayFromStart();
	}
}

void AInteractiveDrawer::OnTimelineUpdate(const float Movement)
{
	if (!ActiveComponent) return;

	const FVector NewLocation = FVector(OriginRelativeLocation.X, CurrentPulloutLenght, OriginRelativeLocation.Z);
	ActiveComponent->SetRelativeLocation(FMath::Lerp(OriginRelativeLocation, NewLocation, Movement));
}

void AInteractiveDrawer::OnTimelineFinished()
{
	if (!InteractComponent) return;
	
	bMoving = false;
	InteractComponent->SetInteractTracePause(false);
}
