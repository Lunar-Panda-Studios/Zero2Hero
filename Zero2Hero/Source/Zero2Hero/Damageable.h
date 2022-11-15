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
	UPROPERTY()
		bool DropAmmo = false;
	UPROPERTY()
		TSubclassOf<AActor> SpawnOnDeath;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		int GetDamage();
	UFUNCTION()
		bool GetIsDead();

	UFUNCTION(BlueprintCallable)
		float GetHealth();
	UFUNCTION(BlueprintCallable)
		float GetMaxHealth();
	UFUNCTION(BlueprintCallable)
		void IncreaseHealth(int amount);
	UFUNCTION(BlueprintCallable)
		void DecreaseHealth(int amount);
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
