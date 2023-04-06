// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemy;
class UBoxComponent;
class ASpaceShip;

UCLASS()
class SPACESHIPBATTLE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<AEnemy> Enemy;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UBoxComponent* SpawnArea;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	int MaxEnemyNum;

	int CurrentEnemyCount;

	FVector GetGenerateLocation();

	void SpawnEnemy();

	ASpaceShip* SpaceShip;

	float MiniDistanceToPlayer = 1200.0f;

	float SpawnInterval;

	FTimerHandle TimerHandle_Spawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DecreaseEnemyCount();

};
