// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedWeapon.h"
#include "MachineGun.generated.h"

UCLASS()
class ZERO2HERO_API AMachineGun : public ARangedWeapon
{
	GENERATED_BODY()
protected:

public:
	AMachineGun();
	UFUNCTION(BlueprintCallable)
		void PrimaryAttack() override;
	UFUNCTION(BlueprintCallable)
		void SecondaryAttack() override;

};
