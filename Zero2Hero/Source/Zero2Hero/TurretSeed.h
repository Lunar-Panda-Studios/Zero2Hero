// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Damageable.h"
#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "TurretSeed.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API ATurretSeed : public AProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> turret;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float seedLifeSpan = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float downCheck = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float seedRadius = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float turretHeight = 200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	TArray<AActor*> ignoreActors;
	TArray<AActor*> actors;
	UClass* seekClass;
	
public:
	ATurretSeed();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void OnHit(AActor* OverlappedActor, AActor* OtherActor);
};
