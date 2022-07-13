// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActors/InteractiveActorBase.h"
#include "InteractiveItem.generated.h"


UCLASS()
class INTERACTPROJECT_API AInteractiveItem : public AInteractiveActorBase
{
	GENERATED_BODY()

	UPROPERTY()
	UStaticMeshComponent* CopyMesh = nullptr;
	bool bPhysicsEnable = false;

protected:

	UPROPERTY()
	UStaticMeshComponent* InspectionMesh = nullptr;

	AInteractiveItem();
	
	virtual void BeginPlay() override;
	virtual void BindToInput();
	virtual void Interact(UInteractComponent* InteractComponent_In) override;
	void StartInspect();
	void FinishInspect();

private:
	void RotateXAxis(const float AxisValue);
	void RotateYAxis(const float AxisValue);
	void SetRotateFactorAxis(float& RotationX, float& RotationY) const;
	UFUNCTION()
	void OnTimelineUpdate(const float Movement);
	UFUNCTION()
	void OnTimelineFinished();
	void MakeNoPhysMeshCopy();
	void RemoveNoPhysMeshCopy() const;
};
