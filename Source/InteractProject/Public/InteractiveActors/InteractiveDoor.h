// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActors/InteractiveFurniture.h"
#include "InteractiveDoor.generated.h"


UCLASS()
class INTERACTPROJECT_API AInteractiveDoor : public AInteractiveFurniture
{
	GENERATED_BODY()

	FRotator DoorHandleRotation = FRotator::ZeroRotator;
	float RotationYaw = 0.0f;
	FTimerHandle PullTimerHandler;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DoorHandleMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTimelineComponent* DoorHandleTimelineComponent = nullptr;

	AInteractiveDoor();
	virtual void BeginPlay() override;
	virtual void Interact(UInteractComponent* InteractComponent_In) override;
	
private:
	void RotateDoor();
	UFUNCTION()
	void OnDoorHandleTimelineUpdate(const float Movement);
	UFUNCTION()
	void OnTimelineUpdate(const float Movement);
	UFUNCTION()
	void OnTimelineFinished();
	
};
