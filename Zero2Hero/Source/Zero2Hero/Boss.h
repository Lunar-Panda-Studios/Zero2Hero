// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Math/UnrealMathUtility.h"
#include "HomingMissile.h"
#include "BossCrystalWeakness.h"
#include "FallingItem.h"
#include "ShockWave.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
		USphereComponent* FireLocationLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
		USphereComponent* FireLocationRight;
	UPROPERTY()
		int Phase = 1;
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
	UPROPERTY(BlueprintReadWrite)
		ABossCrystalWeakness* LeftHandCrystal;
	UPROPERTY(BlueprintReadWrite)
		ABossCrystalWeakness* RightHandCrystal;
	UPROPERTY()
		bool HasPlayed = false;
	UPROPERTY()
		int PhaseSection = 1;
	UPROPERTY()
		FVector HandDownLocation;
	UPROPERTY()
		bool HasHandHitGround;
	UPROPERTY()
		bool LeftHandAlive = true;
	UPROPERTY()
		bool RightHandAlive = true;
	UPROPERTY()
		AShockWave* ShockWaveInstance;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - General")
		TSubclassOf<AShockWave> ShockWaveBP;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - General")
		float DistanceTrace = 1000.0f;
	UPROPERTY()
		bool FirstAnimFinished = false;
	UPROPERTY()
		bool SecondAnimStarted = false;
	UPROPERTY()
		float AttackDelayTimer = 0;
	UPROPERTY()
		float CurrentAttackDelay = 0;
	UPROPERTY()
		bool ShouldShockwave = true;

	//Phase 1 - Delays
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Delays")
		float MA1DelayTo;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Delays")
		float MA2aDelayTo;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Delays")
		float MA2bDelayTo;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Delays")
		float AOE1DelayTo;


	//Phase 1 - Melee Attack 1 Right Arm
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1 Right Arm")
		UAnimSequence* MeleeAttack1RightTo;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1 Right Arm")
		int P1Melee1RDamage;

	//Phase 1 - Melee Attack 1 Left Arm
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1 Left Arm")
		UAnimSequence* MeleeAttack1LeftTo;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 1 Left Arm")
		int P1Melee1LDamage;

	//Phase 1 - Melee Attack 2a Right Arm
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2a Right Arm")
		UAnimSequence* MeleeAttack2aRightTo;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2a Right Arm")
		UAnimSequence* MeleeAttack2aRightReturn;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2a Right Arm")
		float MeleeAttack2aRightTimeDown;
	UPROPERTY()
		float MeleeAttack2aRightTimer = 0;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2a Right Arm")
		int P1Melee2aRDamage;

	//Phase 1 - Melee Attack 2a Left Arm
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2a Left Arm")
		UAnimSequence* MeleeAttack2aLeftTo;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2a Left Arm")
		UAnimSequence* MeleeAttack2aLeftReturn;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2a Left Arm")
		float MeleeAttack2aLeftTimeDown;
	UPROPERTY()
		float MeleeAttack2aLeftTimer = 0;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2a Left Arm")
		int P1Melee2aLDamage;

	//Phase 1 - Melee Attack 2b Right Arm
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2b Right Arm")
		UAnimSequence* MeleeAttack2bRightTo;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2b Right Arm")
		UAnimSequence* MeleeAttack2bRightReturn;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2b Right Arm")
		float MeleeAttack2bRightTimeDown;
	UPROPERTY()
		float MeleeAttack2bRightTimer = 0;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2b Right Arm")
		int P1Melee2bRDamage;

	//Phase 1 - Melee Attack 2b Left Arm
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2b Left Arm")
		UAnimSequence* MeleeAttack2bLeftTo;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2b Left Arm")
		UAnimSequence* MeleeAttack2bLeftReturn;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2b Left Arm")
		float MeleeAttack2bLeftTimeDown;
	UPROPERTY()
		float MeleeAttack2bLeftTimer = 0;
	UPROPERTY(EditAnywhere, Category = "Phase 1 - Melee Attack 2b Left Arm")
		int P1Melee2bLDamage;

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
	UPROPERTY(EditAnywhere, Category = "Phase 2 - General")
		UAnimSequence* BossIdle;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning General")
		USphereComponent* SummonRangeMax;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning General")
		USphereComponent* SummonRangeMin;
	UPROPERTY()
		TArray<AEnemy*> SummonedEnemies;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning General")
		float ZSummonOffSet = 10.0f;
	UPROPERTY()
		bool SpawnSet = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Launcher1Fixed = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Launcher2Fixed = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Harpoon1Launched = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Harpoon2Launched = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isActiveMissile = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isActiveRegProjectile = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ReadyToSpawn = true;

	//Phase 2 - Summon1
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning Version 1")
		TSubclassOf<AEnemy> Summon1EnemyTypeBP;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning Version 1")
		int AmountToSummonV1 = 0;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning Version 1")
		TSubclassOf<AActor> HarponPiece1;
	UPROPERTY()
		int CurrentSummon = 1;

	//Phase 2 - Summon2
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning Version 2")
		TSubclassOf<AEnemy> Summon2EnemyTypeBP;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning Version 2")
		int AmountToSummonV2 = 0;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Summoning Version 2")
		TSubclassOf<AActor> HarponPiece2;

	//Phase 2 - Missile
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Missile Projectile")
		TSubclassOf<AHomingMissile> MissileBP;
	UPROPERTY()
		bool HasFired = false;
	UPROPERTY()
		AHomingMissile* Missile;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Missile Projectile")
		int MissileDamage;
	UPROPERTY()
		float TimerToNext = 0;
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Missile Projectile")
		float TimeToNext = 2.0f;

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
	UPROPERTY(EditAnywhere, Category = "Phase 2 - Regular Projectile")
		int RegProjectileDamage;

public:
	UFUNCTION()
		void SetNewDelay();
	UFUNCTION()
		FVector CalculateSpawnLocation();
	UFUNCTION()
		void CalculateHandLocation();
	UFUNCTION()
		void ShockWave();
	UFUNCTION()
		int HandsAlive();
	UFUNCTION()
		void ShouldEndPhase1();

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
