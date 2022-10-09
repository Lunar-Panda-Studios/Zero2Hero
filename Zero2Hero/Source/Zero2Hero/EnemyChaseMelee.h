// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "EnemyChaseMelee.generated.h"

UCLASS()
class ZERO2HERO_API AEnemyChaseMelee : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyChaseMelee();

	UPROPERTY(EditAnywhere)
		USphereComponent* MeleeCollider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void Attack() override;

	UFUNCTION()
	void OnOverlapMelee(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
