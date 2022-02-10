// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperSideScroller_Player.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "PlayerProjectile.h"
#include "TimerManager.h"


ASuperSideScroller_Player::ASuperSideScroller_Player()
{
	//Sets sprinting to false by default
	bIsSprinting = false;

	// Set our max walk speed to 300.0f
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	
}

void ASuperSideScroller_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//Not always necessary, but good practice to call the function in the base class with super
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Throw", EInputEvent::IE_Pressed, this, &ASuperSideScroller_Player::ThrowProjectile);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ASuperSideScroller_Player::Sprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ASuperSideScroller_Player::StopSprinting);

}

void ASuperSideScroller_Player::Sprint()
{
	if(!bIsSprinting) 
	{
		
		bIsSprinting = true;

		if (bHasPowerupActive)
		{
			GetCharacterMovement()->MaxWalkSpeed = 900.0f;
		} 
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		}

	}
}

void ASuperSideScroller_Player::StopSprinting() {
	if (bIsSprinting)
	{

		bIsSprinting = false;

		if (bHasPowerupActive)
		{
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		}
		
	}
}

void ASuperSideScroller_Player::ThrowProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("Throw Projectile"));
	if (ThrowMontage)
	{
		bool bIsMontagePlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(ThrowMontage);

		if (!bIsMontagePlaying)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(ThrowMontage, 2.0f);
		}
	}
}

void ASuperSideScroller_Player::SpawnProjectile()
{
	if (PlayerProjectile)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			//Spawns the projectile at the ProjectileSocket and sets the rotation to the players rotation
			FVector SpawnLocation = this->GetMesh()->GetSocketLocation(FName("ProjectileSocket"));
			FRotator Rotation = GetActorForwardVector().Rotation();

			APlayerProjectile* Projectile = World->SpawnActor<APlayerProjectile>(PlayerProjectile, SpawnLocation, Rotation, SpawnParams);

			if (Projectile)
			{
				Projectile->CollisionComp->MoveIgnoreActors.Add(SpawnParams.Owner);
			}
		}
	}
}

void ASuperSideScroller_Player::IncrementNumberofCollectables(int32 Value)
{
	if (Value == 0)
	{
		return;
	}
	else
	{
		NumberofCollectables += Value;

		UE_LOG(LogTemp, Warning, TEXT("Number of Coins: %d"), NumberofCollectables);
	}
}

void ASuperSideScroller_Player::IncreasedMovementPowerup()
{
	bHasPowerupActive = true;

	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->JumpZVelocity = 1500.0f;

	UWorld* World = GetWorld();

	if (World)
	{
		World->GetTimerManager().SetTimer(PowerupHandle, this, &ASuperSideScroller_Player::EndPowerup, 8.0f, false);
	}

}

void ASuperSideScroller_Player::EndPowerup()
{
	bHasPowerupActive = false;

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;

	UWorld* World = GetWorld();

	if (World)
	{
		World->GetTimerManager().ClearTimer(PowerupHandle);
	}

}

