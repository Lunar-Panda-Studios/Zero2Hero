// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyPatrol.h"
#include "Reanimator.generated.h"

UCLASS()
class ZERO2HERO_API AReanimator : public AEnemyPatrol
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AReanimator();

protected:
	UPROPERTY(EditAnywhere)
		USphereComponent* ReviveRadius;
	UPROPERTY()
		TArray<AEnemy*> InRangeEnemies;
	UPROPERTY()
		AEnemy* ToRevive;
	UPROPERTY()
		bool isReviving = false;
	UPROPERTY(EditAnywhere)
		float TimeToRevive;
	UPROPERTY()
		float TimerToRevive = 0;
	UPROPERTY(EditAnywhere)
		float RunAwayDistance;
	UPROPERTY()
		float Radius;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
		void OnDeath();
	UFUNCTION()
		float GetRunAwayDistance();
	UFUNCTION()
		void ReanimateEnemy();
	UFUNCTION()
		void FindNewTarget();
	UFUNCTION()
		void OnOverlapInRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnEndOverlapInRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
