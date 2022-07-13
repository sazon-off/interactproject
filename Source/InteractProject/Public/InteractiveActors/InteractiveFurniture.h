// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActors/InteractiveActorBase.h"
#include "InteractiveFurniture.generated.h"


UCLASS()
class INTERACTPROJECT_API AInteractiveFurniture : public AInteractiveActorBase
{
	GENERATED_BODY()

	FTimerHandle PullTimerHandler;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup | Main")
	bool bLocked = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup | Main")
	bool bOpened = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup | Main")
	int32 KeyId = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup | Sound")
	USoundCue* OpenSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup | Sound")
	USoundCue* CloseSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup | Sound")
	USoundCue* LockSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup | Sound")
	USoundCue* UnlockSound = nullptr;


protected:

	bool IsHaveKey() const;
	void TryToUnlock();
	void PullHandle();
	void ResetRelativeRotation();
};
