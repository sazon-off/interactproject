// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActors/InteractiveDoor.h"

#include "Component/InteractComponent.h"
#include "Components/TimelineComponent.h"

AInteractiveDoor::AInteractiveDoor()
{
	DoorHandleMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorHandleMesh");
	DoorHandleMesh->SetupAttachment(StaticMesh);
	DoorHandleTimelineComponent = CreateDefaultSubobject<UTimelineComponent>("DoorHandleTimelineComponent");
}

void AInteractiveDoor::BeginPlay()
{
	Super::BeginPlay();

	//main door timeline
	InitTimeline(TimelineCurveFloat);

	//timeline for handle
	if (!TimelineCurveFloat) return;
	
	FOnTimelineFloat DoorHandleTimelineUpdateDelegate;
	DoorHandleTimelineUpdateDelegate.BindUFunction(this, "OnDoorHandleTimelineUpdate");
	DoorHandleTimelineComponent->AddInterpFloat(TimelineCurveFloat, DoorHandleTimelineUpdateDelegate);
	DoorHandleTimelineComponent->SetTimelineLengthMode(TL_LastKeyFrame);
}

void AInteractiveDoor::Interact(UInteractComponent* InteractComponent_In)
{

	InteractComponent = InteractComponent_In;
	ActiveComponent = InteractComponent->GetActiveComponent();
	
	if (!DoorHandleMesh) return;
	
	DoorHandleRotation = DoorHandleMesh->GetRelativeRotation();
	DoorHandleTimelineComponent->PlayFromStart();

	GetWorldTimerManager().SetTimer(PullTimerHandler, this, &AInteractiveDoor::RotateDoor, 0.2f, false, 0.25f);
	
}

void AInteractiveDoor::RotateDoor()
{
	GetWorldTimerManager().ClearTimer(PullTimerHandler);
	DoorHandleTimelineComponent->ReverseFromEnd();
	if (bMoving) return;
	
	if (bLocked)
	{
		TryToUnlock();
	}
	else
	{
		bMoving = true;
		OriginRelativeRotation = StaticMesh->GetRelativeRotation();
		
		if (FMath::IsNearlyZero(OriginRelativeRotation.Yaw, 1.0f))
		{
			RotationYaw = -90.0f;
			PlayInteractSound(OpenSound);
		}
		else
		{
			RotationYaw = 90.0f;
			PlayInteractSound(CloseSound);
		}

		TimelineComponent->PlayFromStart();
	}
}

void AInteractiveDoor::OnDoorHandleTimelineUpdate(const float Movement)
{
	const FRotator TurnValue = FRotator(DoorHandleRotation.Pitch + 20.0f, DoorHandleRotation.Yaw,
										DoorHandleRotation.Roll);
	const FRotator NewRotation = FMath::Lerp(DoorHandleRotation, TurnValue, Movement);
	DoorHandleMesh->SetRelativeRotation(NewRotation);
}

void AInteractiveDoor::OnTimelineUpdate(const float Movement)
{
	const FRotator TurnValue = FRotator(OriginRelativeRotation.Pitch, OriginRelativeRotation.Yaw + RotationYaw,
										OriginRelativeRotation.Roll);
	const FRotator NewRotation = FMath::Lerp(OriginRelativeRotation, TurnValue, Movement);
	StaticMesh->SetRelativeRotation(NewRotation);
}

void AInteractiveDoor::OnTimelineFinished()
{
	bMoving = false;
}
