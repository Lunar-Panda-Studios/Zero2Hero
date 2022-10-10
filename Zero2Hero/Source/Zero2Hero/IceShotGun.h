// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedWeapon.h"
#include "IceShotGun.generated.h"

UCLASS()
class ZERO2HERO_API AIceShotGun : public ARangedWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIceShotGun();

	UPROPERTY(EditAnywhere, Category = "Ice Shotgun")
		float DegreesFromCentre;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void PrimaryAttack() override;

	UFUNCTION()
		void SecondaryAttack() override;

};
