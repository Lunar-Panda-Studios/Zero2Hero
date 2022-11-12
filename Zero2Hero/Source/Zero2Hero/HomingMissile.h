// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "HomingMissile.generated.h"

UCLASS()
class ZERO2HERO_API AHomingMissile : public AProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHomingMissile();

	UPROPERTY()
		bool isHoming = true;
	UPROPERTY(EditAnywhere)
		float TimeTillExplode = 10;
	UPROPERTY()
		float TimerTilExplode = 0;
	UPROPERTY()
		float ForwardTimer = 0;
	UPROPERTY(EditAnywhere)
		float TimeBeforeLockOn = 0;
	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* ProjectileMoveComp;
	UPROPERTY()
		AActor* Player;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Explode();

};
