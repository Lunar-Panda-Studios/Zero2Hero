// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "Turret.generated.h"

UCLASS()
class ZERO2HERO_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	UPROPERTY()
		TArray<AActor*> ignoreActors;
	UPROPERTY()
		TArray<AActor*> actors;
	UPROPERTY()
		UClass* seekClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float turretRange = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int turretAmmo = 12;
	UPROPERTY()
		int turretChargeUsage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float turretFireRate = 0.3f;
	UPROPERTY()
		float fireRateCooldown = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float damage = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AProjectile> projectile;
	

};
