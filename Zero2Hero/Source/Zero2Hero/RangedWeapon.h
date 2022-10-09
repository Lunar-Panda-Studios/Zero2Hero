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

	UPROPERTY(EditAnywhere, Category = "Ranged Stats")
		int Damage;
	UPROPERTY()
		float Charge;
	UPROPERTY(EditAnywhere, Category = "Ranged Stats")
		float MaxCharge;
	UPROPERTY(EditAnywhere, Category = "Ranged Stats")
		TSubclassOf<AProjectile> Projectile;
	UPROPERTY(EditAnywhere, Category = "Ranged Stats")
		USphereComponent* FireLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PrimaryAttack();
	virtual void SecondaryAttack();

};
