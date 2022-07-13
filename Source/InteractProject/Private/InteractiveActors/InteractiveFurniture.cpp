// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActors/InteractiveFurniture.h"

bool AInteractiveFurniture::IsHaveKey() const
{
	// call check key in inventory func, but inventory not implemented =)
	return false;
}

void AInteractiveFurniture::TryToUnlock()
{
	if (IsHaveKey())
	{
		bLocked = false;
		PlayInteractSound(UnlockSound);
	}
	else
	{
		PlayInteractSound(LockSound);
		PullHandle();
	}
}

// slight twitch mesh 
void AInteractiveFurniture::PullHandle()
{
	if (!ActiveComponent) return;
	
	ActiveComponent->SetRelativeRotation(FRotator(OriginRelativeRotation.Pitch, OriginRelativeRotation.Yaw - 0.5f,
												  OriginRelativeRotation.Roll));
	GetWorldTimerManager().SetTimer(PullTimerHandler, this, &AInteractiveFurniture::ResetRelativeRotation, 0.2f, false, 0.3f);
}

void AInteractiveFurniture::ResetRelativeRotation()
{
	GetWorldTimerManager().ClearTimer(PullTimerHandler);
	if (!ActiveComponent) return;
	ActiveComponent->SetRelativeRotation(OriginRelativeRotation);
}
