// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "PlayerCharacter.h"
#include "Perception/AIPerceptionTypes.h"
#include "Enemy.generated.h"

UCLASS()
class ZERO2HERO_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY()
		bool CanSee = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	/*UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		float AgroRadius;
	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		float DeaggroRadius;*/
	UPROPERTY()
		bool InRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MainBody;
	UPROPERTY(EditAnywhere)
		USphereComponent* PlayerRadius;

	UPROPERTY()
		UAIPerceptionComponent* AIPC;
	UPROPERTY(EditAnywhere)
		UAISenseConfig* SightConfig;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attack();

	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus stimulus);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnMainBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
