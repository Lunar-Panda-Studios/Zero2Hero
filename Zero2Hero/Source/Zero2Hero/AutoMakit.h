// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyPatrol.h"
#include "Projectile.h"
#include "AutoMakit.generated.h"

UCLASS()
class ZERO2HERO_API AAutoMakit : public AEnemyPatrol
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAutoMakit();

	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile> ProjectileBP;
	UPROPERTY(EditAnywhere)
		USphereComponent* FireLocation;
	UPROPERTY(EditAnywhere, Category = "Flying Type")
		float RadiusCanFollow = 4000;
	UPROPERTY()
		FVector StartLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();

};
