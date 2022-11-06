// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangedWeapon.h"
#include "ChargeRifle.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API AChargeRifle : public ARangedWeapon
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	AChargeRifle();

	UFUNCTION(BlueprintCallable)
		void PrimaryAttack() override;
	UFUNCTION(BlueprintCallable)
		void SecondaryAttack() override;
	UFUNCTION(BlueprintCallable)
		void PrimaryAttackEnd() override;
	UFUNCTION()
		void Attack(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fireRate = 5.0f;
	UPROPERTY()
		float currentCooldown = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float fireTime = 2.0f;
	UPROPERTY()
		float currentFireTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float secondaryFireRate = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float secondaryLaunchSpeed = 500.0f;

	UPROPERTY()
		float secondaryCurrentCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool shooting = false;

	UPROPERTY()
		bool hasFired = false;
};
