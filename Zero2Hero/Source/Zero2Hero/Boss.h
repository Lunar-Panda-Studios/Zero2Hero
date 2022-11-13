// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Math/UnrealMathUtility.h"
#include "HomingMissile.h"
#include "BossCrystalWeakness.h"
#include "FallingItem.h"
#include "Boss.generated.h"

UENUM()
enum BossAttacks
{
	//Phase 1
	P1Melee1L	UMETA(DisplayName = "P1Melee1L"),
	P1Melee1R	UMETA(DisplayName = "P1Melee1R"),
	P1Melee2aL	UMETA(DisplayName = "P1Melee2aL"),
	P1Melee2aR	UMETA(DisplayName = "P1Melee2aR"),
	P1Melee2bL	UMETA(DisplayName = "P1Melee2bL"),
	P1Melee2bR	UMETA(DisplayName = "P1Melee2bR"),
	P1AoE1		UMETA(DisplayName = "P1AoE1"),

	//Phase 2
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

	void BeginPlay();

	//Misc
	UPROPERTY(EditAnywhere, Category = "Misc")
		USphereComponent* FireLocationLeft;
	UPROPERTY(EditAnywhere, Category = "Misc")
		USphereComponent* FireLocationRight;
	UPROPERTY()
		int Phase = 2;
	UPROPERTY()
		TEnumAsByte<BossAttacks> CurrentAttack = BossAttacks::Waiting;
	//Will need changing to Skeletal Mesh later
	UPROPERTY()
		UStaticMeshComponent* BodyMesh;
	UPROPERTY()
		bool ShouldDamage = true;

	//Phase 1 General
	UPROPERTY(EditAnywhere, Category = "Phase 1 - General")
		TSubclassOf<ABossCrystalWeakness> LeftHandCrystalBP;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - General")
		TSubclassOf<ABossCrystalWeakness> RightHandCrystalBP;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - General")
		FName LeftHandCrystalSocket;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - General")
		FName RightHandCrystalSocket;
	UPROPERTY()
		ABossCrystalWeakness* LeftHandCrystal;
	UPROPERTY()
		ABossCrystalWeakness* RightHandCrystal;
	UPROPERTY()
		bool HasPlayed = false;
	UPROPERTY()
		int PhaseSection;
	UPROPERTY()
		int HandsAlive = 2;


	//Phase 1 - Melee Attack 1 Right Arm
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1 Right Arm")
		UAnimSequence* MeleeAttack1Right;

	//Phase 1 - Melee Attack 1 Left Arm
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1 Left Arm")
		UAnimSequence* MeleeAttack1Left;

	//Phase 1 - Melee Attack 1a Right Arm
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1a Right Arm")
		UAnimSequence* MeleeAttack1aRight;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1a Right Arm")
		float MeleeAttack1aRightTimeDown;
	UPROPERTY()
		float MeleeAttack1aRightTimer = 0;

	//Phase 1 - Melee Attack 1a Left Arm
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1a Left Arm")
		UAnimSequence* MeleeAttack1aLeft;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1a Right Arm")
		float MeleeAttack1aLeftTimeDown;
	UPROPERTY()
		float MeleeAttack1aLeftTimer = 0;

	//Phase 1 - Melee Attack 1b Right Arm
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1b Right Arm")
		UAnimSequence* MeleeAttack1bRight;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1a Right Arm")
		float MeleeAttack1bRightTimeDown;
	UPROPERTY()
		float MeleeAttack1bRightTimer = 0;

	//Phase 1 - Melee Attack 1b Left Arm
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1b Left Arm")
		UAnimSequence* MeleeAttack1bLeft;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1a Right Arm")
		float MeleeAttack1bLeftTimeDown;
	UPROPERTY()
		float MeleeAttack1bLeftTimer = 0;

	//Phase 1 - AOE1 
	UPROPERTY(EditAnywhere, Category = "Phase 1 - AoE 1")
		TSubclassOf<AFallingItem> FallingItemBP;
	UPROPERTY()
		TArray<AFallingItem*> FallingItems;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - AoE 1")
		TArray<int> AoE1AmountToSpawn;
	UPROPERTY()
		int AoE1ZOffset = 500.0f;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - AoE 1")
		float AoE1DelayBetweenSpawns;
	UPROPERTY()
		float AoE1TimerBetweenSpawns = 0;
	UPROPERTY()
		int AoE1SpawnCounter = 0;

	//Phase2 General

	//Phase 2 - Summoning General
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning General")
		USphereComponent* SummonRangeMax;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning General")
		USphereComponent* SummonRangeMin;
	UPROPERTY()
		TArray<AEnemy*> SummonedEnemies;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning General")
		float ZSummonOffSet = 10.0f;

	//Phase 2 - Summon1
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning Version 1")
		TSubclassOf<AEnemy> Summon1EnemyTypeBP;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning Version 1")
		int AmountToSummonV1 = 0;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning Version 1")
		TSubclassOf<AActor> HarponPiece1;
	UPROPERTY()
		bool HarponPiece1Spawned = false;

	//Phase 2 - Summon2
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning Version 2")
		TSubclassOf<AEnemy> Summon2EnemyTypeBP;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning Version 2")
		int AmountToSummonV2 = 0;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning Version 2")
		TSubclassOf<AActor> HarponPiece2;
	UPROPERTY()
		bool HarponPiece2Spawned = false;

	//Phase 2 - Missile
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Missile Projectile")
		TSubclassOf<AHomingMissile> MissileBP;
	UPROPERTY()
		bool HasFired = false;
	UPROPERTY()
		AHomingMissile* Missile;

	//Phase 2 - Regular Projectile
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Regular Projectile")
		TSubclassOf<AProjectile> RegularProjectileBP;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Regular Projectile")
		float RegularProjectileFireDelay = 2;
	UPROPERTY()
		float RegularProjectileFireTimer = 0;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Regular Projectile")
		int AmountToFire = 0;
	UPROPERTY()
		int AmountHasFired = 0;

public:
	UFUNCTION()
		FVector CalculateSpawnLocation();

	UFUNCTION(BlueprintCallable)
		void Melee1Right();
	UFUNCTION(BlueprintCallable)
		void Melee1Left();
	UFUNCTION(BlueprintCallable)
		void Melee2aRight();
	UFUNCTION(BlueprintCallable)
		void Melee2aLeft();
	UFUNCTION(BlueprintCallable)
		void Melee2bRight();
	UFUNCTION(BlueprintCallable)
		void Melee2bLeft();
	UFUNCTION(BlueprintCallable)
		void AoE1();



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

	UFUNCTION()
		void OnHitArms(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	//UFUNCTION()
	//	void OnHitHands(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
