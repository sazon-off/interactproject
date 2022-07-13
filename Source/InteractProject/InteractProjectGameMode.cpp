// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractProjectGameMode.h"
#include "InteractProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInteractProjectGameMode::AInteractProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = AInteractProjectCharacter::StaticClass();
}
