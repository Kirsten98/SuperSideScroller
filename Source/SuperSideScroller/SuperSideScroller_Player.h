// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuperSideScrollerCharacter.h"
#include "SuperSideScroller_Player.generated.h"

/**
 * 
 */
UCLASS()
class SUPERSIDESCROLLER_API ASuperSideScroller_Player : public ASuperSideScrollerCharacter
{
	GENERATED_BODY()
	

public: 
	ASuperSideScroller_Player();	

	void ThrowProjectile();

	void SpawnProjectile();

	UFUNCTION(BlueprintPure)
	int32 GetCurrentNumberofCollectables()
	{
		return NumberofCollectables;
	}

	void IncrementNumberofCollectables(int32 Value);

	void IncreasedMovementPowerup();

protected:
	//Override base character class function to setup our player input component
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 

	void Sprint();

	void StopSprinting();

	void EndPowerup();

private:
	//Bool to control if we are sprinting. Failsafe
	bool bIsSprinting;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* ThrowMontage;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class APlayerProjectile> PlayerProjectile;

	int32 NumberofCollectables;

	//This timer handle will keep track of how much time has elapsed since it was initiated.
	FTimerHandle PowerupHandle;

	bool bHasPowerupActive;
};
