// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyWondering.h"
#include "BombTosser.generated.h"

UCLASS()
class ZERO2HERO_API ABombTosser : public AEnemyWondering
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABombTosser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile> ProjectileBP;
	UPROPERTY(EditAnywhere)
		USphereComponent* FireLocation;
	UPROPERTY(EditAnywhere, Category = "Flying Type")
		float RadiusCanFollow = 3000.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();

};
