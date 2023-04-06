// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceShip.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class ABullet;
class USoundCue;

UCLASS()
class SPACESHIPBATTLE_API ASpaceShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LookAtCursor();

	APlayerController* pc;

	UPROPERTY(EditAnywhere, Category = "Fire")
	TSubclassOf<ABullet> Bullet;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* SpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* ShipMS;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere,Category="Move")
	float Speed;
	FTimerHandle TimerHandle_BetweenShot;

	UPROPERTY(EditAnywhere, Category = "Fire")
	float TimeBetweenShot;

	FTimerHandle TimerHandle_Restart;

	bool bUpMove;
	bool bRightMove;

	bool bDead;

	void MoveUp(float Value);

	void MoveRight(float Value);

	void Move();

	void Fire();

	void StartFire();

	void EndFire();

	void RestartLevel();

	void OnDeath();

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* GameOverCue;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* ShootCue;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UParticleSystemComponent* ThrusterParticleComp;

	UPROPERTY(EditAnywhere, Category = "Particle")
	UParticleSystem* ExplosionParticle;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* Other) override;

	FORCEINLINE bool GetBDead() {
		return bDead;
	}
};
