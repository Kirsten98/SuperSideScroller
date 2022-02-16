// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperSideScroller_Brick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "SuperSideScroller_Player.h"


// Sets default values
ASuperSideScroller_Brick::ASuperSideScroller_Brick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BrickMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BrickMesh"));
	BrickMesh->SetCollisionProfileName("BlockAll");
	RootComponent = BrickMesh;

	BrickCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BrickCollision"));
	BrickCollision->SetCollisionProfileName("BlockAll");

	BrickCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	BrickCollision->OnComponentHit.AddDynamic(this, &ASuperSideScroller_Brick::OnHit);

}

// Called when the game starts or when spawned
void ASuperSideScroller_Brick::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASuperSideScroller_Brick::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ASuperSideScroller_Player* Player = Cast<ASuperSideScroller_Player>(OtherActor);

	if (Player && bHasCollectable)
	{
		AddCollectable(Player);
		PlayHitSound();
		PlayHitExplosion();
		Destroy();
	}
}

void ASuperSideScroller_Brick::AddCollectable(class ASuperSideScroller_Player* Player)
{
	Player->IncrementNumberofCollectables(CollectableValue);
	
}

void ASuperSideScroller_Brick::PlayHitSound()
{
	UWorld* World = GetWorld();

	if (World)
	{
		if (HitSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(World, HitSound, GetActorLocation());
		}
	}
}

void ASuperSideScroller_Brick::PlayHitExplosion()
{
	UWorld* World = GetWorld();

	if (World)
	{
		if (Explosion)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, Explosion, GetActorTransform());
		}
	}
}

