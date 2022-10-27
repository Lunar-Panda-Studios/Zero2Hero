// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damageable.generated.h"

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
	UPROPERTY()
		float AnimationTime;
	UPROPERTY(EditAnywhere, Category = "Character Stats")
		float AnimationTimer = 1.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		int GetDamage();

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

};
