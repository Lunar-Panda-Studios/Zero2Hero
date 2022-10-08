// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Enemy.generated.h"

UCLASS()
class ZERO2HERO_API AEnemy : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		int Health;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		int Damage;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		float MovementSpeed;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		float CombatMovementSpeed;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		float AttackCooldown;
	UPROPERTY()
		float AttackCooldownTimer;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		float AttackSpeed;
	UPROPERTY()
		float AttackSpeedTimer;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		float DropChance;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		float AgroRadius;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		float DeaggroRadius;
	UPROPERTY()
		bool InRange = false;

	UPROPERTY()
		UCapsuleComponent* MainBody;
	UPROPERTY()
		USphereComponent* PlayerRadius;

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void OnMainBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
