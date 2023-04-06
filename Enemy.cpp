// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceShip.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "ShipGameMode.h"
#include "EnemySpawner.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	ShipMS = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMS"));
	ShipMS->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	SpaceShip = Cast<ASpaceShip>(UGameplayStatics::GetPlayerPawn(this, 0));
	SetColor();
	MyGameMode= Cast<AShipGameMode>( UGameplayStatics::GetGameMode(this));
	TArray<AActor*>EnemySpawnerArray;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawner::StaticClass(), EnemySpawnerArray);
	EnemySpawner = Cast<AEnemySpawner>(EnemySpawnerArray[0]);
}

void AEnemy::MoveTowardsPlayer()
{

	FVector Direction = (SpaceShip->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	AddActorWorldOffset(Direction * Speed * FApp::GetDeltaTime(), true);
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), SpaceShip->GetActorLocation()));
}

void AEnemy::OnDeath()
{
	MyGameMode->IncreaseScore();
	EnemySpawner->DecreaseEnemyCount();
	SpawnExplosion();
	Destroy();
}


// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!SpaceShip->GetBDead())
		MoveTowardsPlayer();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

