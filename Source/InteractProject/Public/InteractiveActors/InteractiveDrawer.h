// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActors/InteractiveFurniture.h"
#include "InteractiveDrawer.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTPROJECT_API AInteractiveDrawer : public AInteractiveFurniture
{
	GENERATED_BODY()

	float CurrentPulloutLenght = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Setup | Main")
	float PulloutLenght = 45.0f;
	
	
protected:
	
	virtual void BeginPlay() override;
	virtual void Interact(UInteractComponent* InteractComponent_In) override;
	
private:

	UFUNCTION()
	void OnTimelineUpdate(const float Movement);
	UFUNCTION()
	void OnTimelineFinished();
	
};
