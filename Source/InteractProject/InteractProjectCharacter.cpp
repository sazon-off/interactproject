// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Component/InteractComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AInteractProjectCharacter::AInteractProjectCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>("CharacterCamera");
	CharacterCamera->SetupAttachment(GetMesh(), "head");
	CharacterCamera->bUsePawnControlRotation = true;
	
	InteractComponent = CreateDefaultSubobject<UInteractComponent>("InteractComponent");
}

void AInteractProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &AInteractProjectCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AInteractProjectCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, InteractComponent, &UInteractComponent::StartInteract);
}

UCameraComponent* AInteractProjectCharacter::GetCharacterCamera() const
{
	return CharacterCamera;
}

void AInteractProjectCharacter::DisableMovement()
{
	const UWorld* World = GetWorld();
	if (!World) return;
	DisableInput(World->GetFirstPlayerController());
	GetMovementComponent()->StopMovementImmediately();
}

void AInteractProjectCharacter::MoveForward(const float Value)
{
	const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);	
}

void AInteractProjectCharacter::MoveRight(const float Value)
{
	const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}