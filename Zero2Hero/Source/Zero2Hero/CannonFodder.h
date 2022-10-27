// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "EnemyWondering.h"
#include "CannonFodder.generated.h"

UCLASS()
class ZERO2HERO_API ACannonFodder : public AEnemyWondering
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACannonFodder();

	UPROPERTY(EditAnywhere)
		USphereComponent* MeleeCollider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		USphereComponent* BlastRadius;
	UPROPERTY(EditAnywhere)
		bool CanExplode = false;
	UPROPERTY(EditAnywhere)
		float KnockbackSpeed;
	UPROPERTY()
		float Timer = 0.0f;
	UPROPERTY(EditAnywhere)
		float TimeToExplode;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void Explode();

	UFUNCTION()
		void OnOverlapBeginExplode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapMelee(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
