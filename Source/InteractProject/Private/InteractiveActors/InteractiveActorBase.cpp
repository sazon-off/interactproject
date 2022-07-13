// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActors/InteractiveActorBase.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AInteractiveActorBase::AInteractiveActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->ComponentTags.Add("Interact");
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>("TimelineComponent");
}

void AInteractiveActorBase::InitTimeline(UCurveFloat* CurveFloat_In)
{
	if (!CurveFloat_In) return;
	
	FOnTimelineFloat TimelineUpdateDelegate;
	FOnTimelineEventStatic TimelineFinishedDelegate;
	TimelineUpdateDelegate.BindUFunction(this, "OnTimelineUpdate");
	TimelineFinishedDelegate.BindUFunction(this, "OnTimelineFinished");
	TimelineComponent->AddInterpFloat(CurveFloat_In, TimelineUpdateDelegate);
	TimelineComponent->SetTimelineFinishedFunc(TimelineFinishedDelegate);
	TimelineComponent->SetTimelineLengthMode(TL_LastKeyFrame);
}

void AInteractiveActorBase::PlayInteractSound(USoundCue* SoundCue_In) const
{
	const UWorld* World = GetWorld();
	if (!World || !SoundCue_In) return;
	UGameplayStatics::PlaySoundAtLocation(World, SoundCue_In, GetActorLocation());
}


