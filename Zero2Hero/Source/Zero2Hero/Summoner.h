// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyPatrol.h"
#include "Summoner.generated.h"

UCLASS()
class ZERO2HERO_API ASummoner : public AEnemyPatrol
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASummoner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AEnemy> EnemyToSpawn;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AEnemy> StrongerEnemyToSpawn;
	UPROPERTY(EditAnywhere)
		float ChanceOfSpawningStrong;
	UPROPERTY(EditAnywhere)
		int AmountToSummon;
	UPROPERTY(EditAnywhere)
		USphereComponent* SummonRange;
	UPROPERTY(EditAnywhere)
		float TimeToSummon;
	UPROPERTY(EditAnywhere)
		float SummonCooldown;
	UPROPERTY()
		float CooldownTimer = 0.0f;
	UPROPERTY()
		float TimerToSummon;
	UPROPERTY()
		bool CanSummon = true;
	UPROPERTY(EditAnywhere)
		float RunAwayDistance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Summon();
	float GetRunAwayDistance();

};
