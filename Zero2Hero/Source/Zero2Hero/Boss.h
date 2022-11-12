// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Math/UnrealMathUtility.h"
#include "HomingMissile.h"
#include "Boss.generated.h"

UENUM()
enum BossAttacks
{
	P2RegularProjectile  UMETA(DisplayName = "P2RegularProjectile"),
	P2MissileProjectile UMETA(DisplayName = "P2MissileProjectile"),
	P2SummonV1  UMETA(DisplayName = "P2SummonV1"),
	P2SummonV2 UMETA(DisplayName = "P2SummonV2"),
	Waiting UMETA(DisplayName = "Waiting"),
};
UCLASS()
class ZERO2HERO_API ABoss : public AEnemy
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	ABoss();

	//Misc
	UPROPERTY(EditAnywhere, Category = "Misc")
		USphereComponent* FireLocationLeft;
	UPROPERTY(EditAnywhere, Category = "Misc")
		USphereComponent* FireLocationRight;
	UPROPERTY()
		int Phase = 2;
	UPROPERTY()
		TEnumAsByte<BossAttacks> CurrentAttack = BossAttacks::Waiting;

	//Phase2 General

	//Phase 2 - Summoning General
	UPROPERTY(EditAnywhere, Category = "Summoning General")
		USphereComponent* SummonRangeMax;
	UPROPERTY(EditAnywhere, Category = "Summoning General")
		USphereComponent* SummonRangeMin;
	UPROPERTY()
		TArray<AEnemy*> SummonedEnemies;

	//Phase 2 - Summon1
	UPROPERTY(EditAnywhere, Category = "Summoning Version 1")
		TSubclassOf<AEnemy> Summon1EnemyTypeBP;
	UPROPERTY(EditAnywhere, Category = "Summoning Version 1")
		int AmountToSummonV1 = 0;
	UPROPERTY(EditAnywhere, Category = "Summoning Version 1")
		TSubclassOf<AActor> HarponPiece1;
	UPROPERTY()
		bool HarponPiece1Spawned = false;

	//Phase 2 - Summon2
	UPROPERTY(EditAnywhere, Category = "Summoning Version 2")
		TSubclassOf<AEnemy> Summon2EnemyTypeBP;
	UPROPERTY(EditAnywhere, Category = "Summoning Version 2")
		int AmountToSummonV2 = 0;
	UPROPERTY(EditAnywhere, Category = "Summoning Version 2")
		TSubclassOf<AActor> HarponPiece2;
	UPROPERTY()
		bool HarponPiece2Spawned = false;

	//Phase 2 - Missile
	UPROPERTY(EditAnywhere, Category = "Missile Projectile")
		TSubclassOf<AHomingMissile> MissileBP;
	UPROPERTY()
		bool HasFired = false;
	UPROPERTY()
		AHomingMissile* Missile;

	//Phase 2 - Regular Projectile
	UPROPERTY(EditAnywhere, Category = "Regular Projectile")
		TSubclassOf<AProjectile> RegularProjectileBP;
	UPROPERTY(EditAnywhere, Category = "Regular Projectile")
		float RegularProjectileFireDelay = 2;
	UPROPERTY()
		float RegularProjectileFireTimer = 0;
	UPROPERTY(EditAnywhere, Category = "Regular Projectile")
		int AmountToFire = 0;
	UPROPERTY()
		int AmountHasFired = 0;

public:
	UFUNCTION(BlueprintCallable)
		void SummonType1();
	UFUNCTION(BlueprintCallable)
		void SummonType2();
	UFUNCTION(BlueprintCallable)
		void MissileAttack();
	UFUNCTION(BlueprintCallable)
		void ProjectileAttack();
	UFUNCTION(BlueprintCallable)
		void ChangePhase();
	UFUNCTION()
		void Phase2AttackChoice();
	UFUNCTION()
		void HarponSpawn();


	
};
