// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractProjectCharacter.generated.h"

class UCameraComponent;
class UInteractComponent;
UCLASS(config=Game)
class AInteractProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CharacterCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInteractComponent* InteractComponent;

protected:

	AInteractProjectCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void MoveForward(const float Value);
	void MoveRight(const float Value);

public:
	
	UCameraComponent* GetCharacterCamera() const;
	void DisableMovement();
	
};

