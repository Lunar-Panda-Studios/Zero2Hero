// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "GameManager.h"
#include "AllEnemiesDefeated.generated.h"

UCLASS()
class ZERO2HERO_API AAllEnemiesDefeated : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAllEnemiesDefeated();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<AEnemy*> EnemiesToCheck;
	int CurrentSummon = 1;
	TSubclassOf<AActor> Harpoon1;
	TSubclassOf<AActor> Harpoon2;
	UGameManager* Manager;
	bool SpawnSet = false;
	bool InUse = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSpawnSet(bool NewSpawnSet);
	void SetInUse(bool newInUse);
	void AddEnemy(AEnemy* Enemy);
	void CheckEnemyStatus();
	void SetCurrentSummon(int newSummonV);
	void SetHarpoon1(TSubclassOf<AActor> Harpoon);
	void SetHarpoon2(TSubclassOf<AActor> Harpoon);

};
