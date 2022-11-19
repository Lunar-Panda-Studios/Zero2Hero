// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Camera.h"
#include "RangedWeapon.generated.h"

UCLASS()
class ZERO2HERO_API ARangedWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangedWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite)
		int WeaponType;
	UPROPERTY(EditAnywhere, Category = "Ranged Stats", BlueprintReadWrite)
		int Damage;
	UPROPERTY(EditAnywhere, Category = "Ranged Stats", BlueprintReadWrite)
		int ChargeUsage = 10;
	UPROPERTY(EditAnywhere, Category = "Ranged Stats", BlueprintReadWrite)
		int SecondaryChargeUsage = 30;
	UPROPERTY(BlueprintReadWrite)
		int Charge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Stats")
		float CurrentAmmo = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Stats")
		float AmmoMax = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Stats")
		float MaxCharge = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Stats")
		TSubclassOf<AProjectile> Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Stats")
		TSubclassOf<AActor> SecondaryProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Stats")
		USphereComponent* FireLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Stats")
		float TimeToReload = 0.0f;
	UPROPERTY(BlueprintReadWrite)
		float TimerReload = 0.0f;
	UPROPERTY()
		ACamera* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Stats")
		float CameraAimDifference = 15.0f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PrimaryAttack();
	virtual void PrimaryAttackEnd();
	virtual void SecondaryAttack();

	UFUNCTION(BlueprintCallable)
		int GetCharge();
	UFUNCTION(BlueprintCallable)
		void IncreaseCharge(int amount);
	UFUNCTION(BlueprintCallable)
		bool DecreaseCharge(int amount);
	UFUNCTION(BlueprintCallable)
		int GetUsage();

	UFUNCTION(BlueprintCallable)
		float GetAmmo();
	UFUNCTION(BlueprintCallable)
		void IncreaseAmmo(int amount);
	UFUNCTION(BlueprintCallable)
		void DecreaseAmmo(int amount);
	UFUNCTION()
		bool AmmoCheck();
	UFUNCTION(BlueprintCallable)
		bool Reload();
	UFUNCTION()
		float GetTimerReload();
	UFUNCTION()
		void SetTimerReload(float amount);
	UFUNCTION()
		float GetTimeToReload();
	UFUNCTION(BlueprintCallable)
		void SetCamera(ACamera* newCamera);
	UFUNCTION(BlueprintImplementableEvent)
		void Reloading();

	UFUNCTION(BlueprintImplementableEvent)
		void OnFire();

};
