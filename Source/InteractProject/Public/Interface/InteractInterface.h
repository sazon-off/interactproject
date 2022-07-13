// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

class UInteractComponent;

UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class INTERACTPROJECT_API IInteractInterface
{
	GENERATED_BODY()

public:

	virtual void Interact(UInteractComponent* InteractComponent_In);
};
