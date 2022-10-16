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
		float DegreesAroundCentre;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Ice Shotgun")
		int IcicleNumber;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void PrimaryAttack() override;

	UFUNCTION(BlueprintCallable)
		void SecondaryAttack() override;

};
