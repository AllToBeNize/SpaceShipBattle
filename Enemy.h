// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

class USphereComponent;
class ASpaceShip;
class AShipGameMode;
class AEnemySpawner;

UCLASS()
class SPACESHIPBATTLE_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* ShipMS;

	void MoveTowardsPlayer();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnExplosion();

	UFUNCTION(BlueprintImplementableEvent)
	void SetColor();

	UPROPERTY(EditAnywhere, Category = "Particle")
	UParticleSystem* ExplosionParticle;

	AShipGameMode* MyGameMode;

	ASpaceShip* SpaceShip;

	float Speed = 300.0f;

	AEnemySpawner* EnemySpawner;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnDeath();
};
