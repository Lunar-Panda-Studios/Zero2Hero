// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager.h"
#include "Damageable.generated.h"

UENUM()
enum ElementType
{
	Ice  UMETA(DisplayName = "Ice"),
	Fire UMETA(DisplayName = "Fire"),
	Electric  UMETA(DisplayName = "Electric"),
	Nature UMETA(DisplayName = "Nature"),
	None UMETA(DisplayName = "None"),
};

UCLASS()
class ZERO2HERO_API ADamageable : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADamageable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
		float MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
		int Damage;
	UPROPERTY()
		bool isDead = false;
	UPROPERTY(BlueprintReadWrite)
		bool beingRevived = false;
	UPROPERTY(EditAnywhere, Category = "Character Stats")
		float AnimationTime;
	UPROPERTY()
		float AnimationTimer = 0.0f;
	UPROPERTY()
		bool isShielded = false;
	UPROPERTY()
		TEnumAsByte<ElementType> CurrentShieldType = ElementType::None;
	UPROPERTY()
		bool ReflectorShield = false;
	UPROPERTY()
		ADamageable* PairedEnemy;
	UPROPERTY()
		UGameManager* Manager;
	UPROPERTY()
		bool Allow = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool DropAmmo = false;
	UPROPERTY()
		TSubclassOf<AActor> SpawnOnDeath;
	UPROPERTY()
		bool IsCrystal = false;
	UPROPERTY()
		bool isBoss = false;
	UPROPERTY(BlueprintReadWrite)
		bool CanDamage = true;
	UPROPERTY()
		bool HasSpawned = false;
	UPROPERTY()
		bool CanSpawn = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SetCanSpawn(bool newSpawn);

	UFUNCTION()
		int GetDamage();
	UFUNCTION()
		bool GetIsDead();
	UFUNCTION()
		bool GetBeingRevived();
	UFUNCTION()
		void SetBeingRevived(bool newRevive);
	UFUNCTION()
		void SetIsDead(bool newDead);
	UFUNCTION()
		void SetIsCrystal(bool NewIsCrystal);
	UFUNCTION()
		virtual void OnDeath();

	UFUNCTION(BlueprintCallable)
		float GetHealth();
	UFUNCTION(BlueprintCallable)
		float GetMaxHealth();
	UFUNCTION(BlueprintCallable)
		void IncreaseHealth(int amount);
	UFUNCTION(BlueprintCallable)
		void DecreaseHealth(int amount);
	UFUNCTION()
		void SetHealth(int amount);
	UFUNCTION()
		void CheckDeath();
	UFUNCTION()
		bool GetIsShielded();
	UFUNCTION()
		bool GetIsShieldReflect();
	UFUNCTION()
		TEnumAsByte<ElementType> GetShieldType();
	UFUNCTION()
		void UnshieldEnemy();
	UFUNCTION()
		void SetEnemyPair(ADamageable* newPair);
	UFUNCTION()
		void SetShielded(bool shield);
	UFUNCTION()
		void UnPair();
	UFUNCTION()
		void SetShieldType(TEnumAsByte<ElementType> newElement);
	UFUNCTION()
		void SetisReflectorShield(bool isReflector);
	UFUNCTION()
		void SetSpawnOnDeath(TSubclassOf<AActor> newDropItem);

	UFUNCTION(BlueprintImplementableEvent)
		void EnemyDamaged();
	UFUNCTION(BlueprintImplementableEvent)
		void EnemyDies();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayerDamaged();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayerDies();

};
