// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyPatrol.h"
#include "Projectile.h"
#include "Buffer.generated.h"

UCLASS()
class ZERO2HERO_API ABuffer : public AEnemyPatrol
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABuffer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		bool isPaired = false;
	UPROPERTY()
		float TimerForEachType = 0.0f;
	UPROPERTY(EditAnywhere)
		float TimeForEachType;
	UPROPERTY(EditAnywhere)
		USphereComponent* TargetRange;
	UPROPERTY()
		TArray<ADamageable*> TargetsInRange;
	UPROPERTY(EditAnywhere)
		USphereComponent* MeleeCollider;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile> ProjectileBP;
	UPROPERTY(EditAnywhere)
		USphereComponent* FireLocation;
	UPROPERTY(EditAnywhere)
		bool isReflectorVariant = false;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool SeekNewTarget();
	void MeleeAttack(float DeltaTime);
	void RangedAttack();

	UFUNCTION()
	void OnBeginOverlapTargetRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlapTargetRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
