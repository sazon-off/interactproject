// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "InteractiveActorBase.generated.h"

class USoundCue;
class UInteractComponent;
class UTimelineComponent;
UCLASS()
class INTERACTPROJECT_API AInteractiveActorBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTimelineComponent* TimelineComponent = nullptr;
	UPROPERTY()
	UInteractComponent* InteractComponent = nullptr;
	UPROPERTY()
	UPrimitiveComponent* ActiveComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Setup | Timeline")
	UCurveFloat* TimelineCurveFloat = nullptr;
	bool bMoving = false;
	bool bReverseFinished = false;
	FVector OriginRelativeLocation = FVector::ZeroVector;
	FRotator OriginRelativeRotation = FRotator::ZeroRotator;
	FVector OriginWorldLocation = FVector::ZeroVector;
	FRotator OriginWorldRotation = FRotator::ZeroRotator;


public:	
	AInteractiveActorBase();
	
	virtual void InitTimeline(UCurveFloat* CurveFloat_In);
	void PlayInteractSound(USoundCue* SoundCue_In) const;
};
