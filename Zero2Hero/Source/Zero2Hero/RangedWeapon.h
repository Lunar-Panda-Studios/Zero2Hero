// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "Components/SphereComponent.h"
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

	UPROPERTY(EditAnywhere, Category = "Ranged Stats")
		int Damage;
	UPROPERTY(EditAnywhere, Category = "Ranged Stats")
		int ChargeUsage = 10;
	UPROPERTY()
		int Charge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Stats")
		float MaxCharge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Stats")
		TSubclassOf<AProjectile> Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Stats")
		USphereComponent* FireLocation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PrimaryAttack();
	virtual void SecondaryAttack();

	UFUNCTION(BlueprintCallable)
		int GetCharge();
	UFUNCTION(BlueprintCallable)
		void IncreaseCharge(int amount);
	UFUNCTION(BlueprintCallable)
		bool DecreaseCharge(int amount);
	UFUNCTION(BlueprintCallable)
		int GetUsage();

};
