// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/InteractInterface.h"
#include "InteractComponent.generated.h"

class AInteractProjectCharacter;
class UCameraComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateInteractWidgetSignature, ESlateVisibility);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTPROJECT_API UInteractComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:
	FOnUpdateInteractWidgetSignature OnUpdateInteractWidget;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float InspectDistance = 40.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float InteractDistance = 100.0f;

private:

	FTimerHandle InteractTraceTimerHandle;
	bool bInspectorActivated = false;
	bool bPauseInteractTrace = false;
	
	UPROPERTY()
	AInteractProjectCharacter* PlayerCharacter = nullptr;
	UPROPERTY()
	UCameraComponent* PlayerCamera = nullptr;
	UPROPERTY()
	AActor* ActorInFocus = nullptr;
	UPROPERTY()
	UPrimitiveComponent* ActiveComponent = nullptr;

public:
	
	void StartInteract();
	FVector GetCamLocationInInspectMode() const;
	FRotator GetCamRotationInInspectMode() const;
	UPrimitiveComponent* GetActiveComponent() const;
	void SetInteractTracePause(const bool bPaused);
	void ToggleInspectMode(const bool bActivated);

protected:
	
	UInteractComponent();
	virtual void BeginPlay() override;

private:
	
	void InteractTrace();
};