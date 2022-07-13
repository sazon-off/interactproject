// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActors/InteractiveItem.h"
#include "Component/InteractComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"

AInteractiveItem::AInteractiveItem()
{
	StaticMesh->ComponentTags.Add("Item");
}

void AInteractiveItem::BeginPlay()
{
	Super::BeginPlay();

	Super::BeginPlay();
	
	InitTimeline(TimelineCurveFloat);
	BindToInput();
}

//change control to mesh in inspect mode
void AInteractiveItem::BindToInput()
{
	InputComponent = NewObject<UInputComponent>(this);
	
	if (!InputComponent) return;
	
	InputComponent->RegisterComponent();
	InputComponent->BindAction("Interact", IE_Pressed, this, &AInteractiveItem::FinishInspect);
	InputComponent->BindAxis("Turn", this,&AInteractiveItem::RotateXAxis);
	InputComponent->BindAxis("LookUp", this,&AInteractiveItem::RotateYAxis);
}

void AInteractiveItem::Interact(UInteractComponent* InteractComponent_In)
{
	if (!InteractComponent_In) return;

	InteractComponent = InteractComponent_In;

	StartInspect();
}

void AInteractiveItem::StartInspect()
{
	//if physics is enabled create a copy of mesh without physics
	if (StaticMesh->IsSimulatingPhysics())
	{
		bPhysicsEnable = true;
		MakeNoPhysMeshCopy();
	}
	else
	{
		InspectionMesh = StaticMesh;
	}
	
	OriginRelativeLocation = InspectionMesh->GetRelativeLocation();

	FVector Min;
	FVector Max;
	InspectionMesh->GetLocalBounds(Min, Max);
	
	//slight offset for correct rotation
	InspectionMesh->SetRelativeLocation(FVector(0.0f, 0.0f, Max.Z / -2));
	InspectionMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	if (bPhysicsEnable)
	{
		OriginWorldRotation = StaticMesh->GetComponentRotation();
		OriginWorldLocation = StaticMesh->GetComponentLocation();
	}
	else
	{
		OriginWorldRotation = GetActorRotation();
		OriginWorldLocation = GetActorLocation();
	}
	TimelineComponent->PlayFromStart();
	bReverseFinished = false;
}

void AInteractiveItem::FinishInspect()
{
	const UWorld* World = GetWorld();
	if (!World) return;
	DisableInput(World->GetFirstPlayerController());
	InspectionMesh->SetRelativeLocation(FVector::ZeroVector);
	TimelineComponent->ReverseFromEnd();
	bReverseFinished = true;
}

void AInteractiveItem::RotateXAxis(const float AxisValue)
{
	AddActorWorldRotation(FRotator(0.0f, AxisValue * -1.0f, 0.0f));
}

void AInteractiveItem::RotateYAxis(const float AxisValue)
{
	float AxisX;
	float AxisY;
	SetRotateFactorAxis(AxisX, AxisY);
	AddActorWorldRotation(FRotator(AxisValue * AxisY, 0.0f, AxisValue * AxisX));
}

void AInteractiveItem::SetRotateFactorAxis(float& RotationX, float& RotationY) const
{
	if (!InteractComponent) return;

	const float YawRotator = InteractComponent->GetCamRotationInInspectMode().Yaw;

	//player camera yaw rotation quartering and range matching
	
	if (YawRotator >= -90.0f && YawRotator <= 0.0f)
	{
		RotationX = FMath::GetMappedRangeValueClamped(FVector2D(-90.0f, 0.0f), FVector2D(1.0f, 0.0f), YawRotator);
		RotationY = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, -90.0f), FVector2D(1.0f, 0.0f), YawRotator);
	}
	else if (YawRotator >= -180.0f && YawRotator <= -90.0f)
	{
		RotationX = FMath::GetMappedRangeValueClamped(FVector2D(-90.0f, -180.0f), FVector2D(1.0f, 0.0f), YawRotator);
		RotationY = FMath::GetMappedRangeValueClamped(FVector2D(-90.0f, -180.0f), FVector2D(0.0f, -1.0f), YawRotator);
	}
	else if (YawRotator >= 90.0f && YawRotator <= 180.0f)
	{
		RotationX = FMath::GetMappedRangeValueClamped(FVector2D(90.0f, 180.0f), FVector2D(-1.0f, 0.0f), YawRotator);
		RotationY = FMath::GetMappedRangeValueClamped(FVector2D(90.0f, 180.0f), FVector2D(0.0f, -1.0f), YawRotator);
	}
	else
	{
		RotationX = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 90.0f), FVector2D(0.0f, -1.0f), YawRotator);
		RotationY = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 90.0f), FVector2D(1.0f, 0.0f), YawRotator);
	}
}

void AInteractiveItem::OnTimelineUpdate(const float Movement)
{
	const FVector NewLocation = FMath::Lerp(OriginWorldLocation, InteractComponent->GetCamLocationInInspectMode(), Movement);
	SetActorLocationAndRotation(NewLocation, OriginWorldRotation);
}

void AInteractiveItem::OnTimelineFinished()
{
	const UWorld* World = GetWorld();
	if (!World) return;
	if (!bReverseFinished)
	{
		EnableInput(World->GetFirstPlayerController());
	}
	else
	{
		InteractComponent->ToggleInspectMode(false);
		UGameplayStatics::GetPlayerPawn(World, 0)->GetInstigator()->EnableInput(World->GetFirstPlayerController());
		InteractComponent->SetInteractTracePause(false);
		if (bPhysicsEnable)
		{
			RemoveNoPhysMeshCopy();
		}
		else
		{
			InspectionMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		}
	}
}

void AInteractiveItem::MakeNoPhysMeshCopy()
{
	CopyMesh = NewObject<UStaticMeshComponent>(this);
	CopyMesh->bAutoRegister = true;
	CopyMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CopyMesh->SetStaticMesh(StaticMesh->GetStaticMesh());
	CopyMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CopyMesh->RegisterComponent();
	InspectionMesh = CopyMesh;
	StaticMesh->SetVisibility(false);
	StaticMesh->SetSimulatePhysics(false);
}

void AInteractiveItem::RemoveNoPhysMeshCopy() const
{
	StaticMesh->SetVisibility(true);
	StaticMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	StaticMesh->SetSimulatePhysics(true);
	CopyMesh->UnregisterComponent();
	CopyMesh->DestroyComponent();
}
