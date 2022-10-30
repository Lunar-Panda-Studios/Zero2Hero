// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Icicle.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API AIcicle : public AProjectile
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
};
