// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/BlockingVolume.h"
#include "Enemy.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	BulletMS = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMS->SetupAttachment(RootComponent);

	ProjectileMovementComp = CreateAbstractDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);
	AEnemy* Enemy = Cast<AEnemy>(Other);
	if (Enemy) {
		Enemy->OnDeath();

		Destroy();
	}
	else if (Cast<ABlockingVolume>(Other)) {
		Destroy();
	}
}

