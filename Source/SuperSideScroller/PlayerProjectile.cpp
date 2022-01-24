// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyBase.h"
#include "Components/AudioComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerProjectile::APlayerProjectile()
{
 
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("BlockAll");
	CollisionComp->OnComponentHit.AddDynamic(this, &APlayerProjectile::OnHit);

	//Set root
	RootComponent = CollisionComp;

	//Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 500.0f;
	ProjectileMovement->MaxSpeed = 500.0f;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	ProjectileMovementSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ProjectileMovementSound"));
	ProjectileMovementSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	ProjectileEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Projectile Effect"));
	ProjectileEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	DestroyEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Destroy Effect"));
	DestroyEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	DestroySound = CreateDefaultSubobject<UAudioComponent>(TEXT("Destroy Sound"));
	DestroySound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	InitialLifeSpan = 3.0f;
}

void APlayerProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	UE_LOG(LogTemp, Warning, TEXT("HIT"));

	AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);

	if (Enemy)
	{
		Enemy->DestroyEnemy();
	}

	ExplodeProjectile();
}

void APlayerProjectile::ExplodeProjectile()
{

	UGameplayStatics::SpawnSoundAttached(DestroySound->Sound, RootComponent, FName("DestroySound"), this->GetActorLocation(), EAttachLocation::KeepWorldPosition, true, 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
	UGameplayStatics::SpawnEmitterAttached(DestroyEffect->Template, RootComponent, FName("DestroyEffect"), this->GetActorLocation(), this->GetActorRotation(), this->GetActorScale(), EAttachLocation::KeepWorldPosition, true, EPSCPoolMethod::None, true);

	Destroy();
}