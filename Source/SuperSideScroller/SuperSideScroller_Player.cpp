// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperSideScroller_Player.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"

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

		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

void ASuperSideScroller_Player::StopSprinting() {
	if (bIsSprinting)
	{

		bIsSprinting = false;

		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
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