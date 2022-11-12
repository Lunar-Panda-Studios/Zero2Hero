// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedWeapon.h"
#include "TurretSeed.h"
#include "MachineGun.generated.h"

UCLASS()
class ZERO2HERO_API AMachineGun : public ARangedWeapon
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	AMachineGun();
	
	UFUNCTION(BlueprintCallable)
		void PrimaryAttack() override;
	UFUNCTION(BlueprintCallable)
		void SecondaryAttack() override;
	UFUNCTION(BlueprintCallable)
		void PrimaryAttackEnd() override;
	UFUNCTION()
		void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fireRate = 1.0f;
	UPROPERTY()
		float currentCooldown = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float secondaryCooldown = 2.0f;
	UPROPERTY()
		float currentSecondaryCooldown = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool shooting = false;

};
