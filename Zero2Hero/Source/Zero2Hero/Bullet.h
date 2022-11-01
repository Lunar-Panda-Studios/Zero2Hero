// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Bullet.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API ABullet : public AProjectile
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
};
