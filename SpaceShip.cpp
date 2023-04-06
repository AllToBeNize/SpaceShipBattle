// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShip.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystem.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Bullet.h"
#include "Enemy.h"

// Sets default values
ASpaceShip::ASpaceShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	ShipMS = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMS"));
	ShipMS->SetupAttachment(RootComponent);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComp);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(ShipMS);

	ThrusterParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ThrustParticleComp"));
	ThrusterParticleComp->SetupAttachment(ShipMS);

	Speed = 2500.0f;
	TimeBetweenShot = 0.2f;

	bDead = false;
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();

	pc = Cast<APlayerController>(GetController());
	pc->bShowMouseCursor = true;
}

void ASpaceShip::LookAtCursor()
{
	FVector MouseLocation, MouseDirection;
	pc->DeprojectMousePositionToWorld(MouseLocation,MouseDirection);
	FVector TargetLocation = FVector(MouseLocation.X, MouseLocation.Y, GetActorLocation().Z);
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	SetActorRotation(Rotator);
}

void ASpaceShip::MoveUp(float Value)
{
	if (Value) {
		bUpMove = true;
	}
	else {
		bUpMove = false;
	}
	AddMovementInput(FVector::ForwardVector, Value);

}

void ASpaceShip::MoveRight(float Value)
{
	if (Value) {
		bRightMove = true;
	}
	else {
		bRightMove = false;
	}
	AddMovementInput(FVector::RightVector, Value);
}

void ASpaceShip::Move()
{
	AddActorWorldOffset(ConsumeMovementInputVector() * Speed * FApp::GetDeltaTime(), true);
}

void ASpaceShip::Fire()
{
	if (Bullet && !bDead) {
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<ABullet>(Bullet, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation(), SpawnParams);
		if (ShootCue) {
			UGameplayStatics::PlaySoundAtLocation(this, ShootCue, GetActorLocation());
		}
	}
}

void ASpaceShip::StartFire()
{

	GetWorldTimerManager().SetTimer(TimerHandle_BetweenShot, this, &ASpaceShip::Fire, TimeBetweenShot, true, 0.0f);
}

void ASpaceShip::EndFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BetweenShot);
}

void ASpaceShip::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, "MainMap");
}

void ASpaceShip::OnDeath()
{
	bDead = true;
	CollisionComp->SetVisibility(false,true);
	if (GameOverCue) {
		UGameplayStatics::PlaySoundAtLocation(this, GameOverCue, GetActorLocation());
	}
	if (ExplosionParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), FRotator::ZeroRotator, true);
	}
	GetWorldTimerManager().SetTimer(TimerHandle_Restart, this, &ASpaceShip::RestartLevel, 2.0f, false);
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bDead) {
		if (bUpMove || bRightMove) {
			ThrusterParticleComp->Activate();
		}
		else {
			ThrusterParticleComp->Deactivate();
		}
		LookAtCursor();
		Move();
	}
	else
	{
		ThrusterParticleComp->Deactivate();
	}
}

// Called to bind functionality to input
void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveUp", this, &ASpaceShip::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceShip::MoveRight);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASpaceShip::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASpaceShip::EndFire);
}

void ASpaceShip::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);
	AEnemy* Enemy = Cast<AEnemy>(Other);
	if (Enemy) {
		Enemy->Destroy();
		OnDeath();
		//Destroy();
	}
}

