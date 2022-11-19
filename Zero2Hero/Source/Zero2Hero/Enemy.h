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
#include "GameFramework/CharacterMovementComponent.h"
#include "Damageable.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.generated.h"

UCLASS()
class ZERO2HERO_API AEnemy : public ADamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "AI")
		UBehaviorTree* BT;
	UPROPERTY(EditAnywhere, Category = "AI")
		UBlackboardComponent* BBC;
	UPROPERTY(EditAnywhere, Category = "AI")
		UBehaviorTreeComponent* BTC;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		float MovementSpeed = 10.0f;
	//UPROPERTY(EditAnywhere, Category = "Enemy Stats")
	//	float CombatMovementSpeed;
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
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* NiagaraComp;
	UPROPERTY()
		bool InRange = false;
	UPROPERTY()
		bool CanSee = false;
	UPROPERTY()
		float FlameThrowerDamageTimer = 0.0f;
	UPROPERTY()
		float FlameThrowerDamageTimerMax = 1.0f;
	UPROPERTY()
		bool OnFire = false;
	UPROPERTY()
		int FlameDamage;
	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		bool ShouldMove = true;
	UPROPERTY(EditAnywhere, Category = "Flying Type")
		float DistanceFromGround = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Flying Type")
		float RaydownLength = 1000.0f;
	UPROPERTY()
		bool ZMoveAtStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MainBody;
	UPROPERTY(EditAnywhere)
		USphereComponent* PlayerRadius;

	UPROPERTY()
		UAIPerceptionComponent* AIPC;
	UPROPERTY(EditAnywhere)
		UAISenseConfig* SightConfig;

	UPROPERTY(EditAnywhere)
		UCharacterMovementComponent* MovementComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attack();

	UFUNCTION()
		bool GetCanSee();
	UFUNCTION()
		void SetOnFire(bool isOnFire);
	UFUNCTION()
		void SetFlameDamage(int amount);
	UFUNCTION()
		bool GetInRange();
	UFUNCTION()
		UNiagaraComponent* GetNiagaraComp();
	UFUNCTION()
		float GetMovementSpeed();
	UFUNCTION()
		bool GetShouldMove();
	UFUNCTION()
		bool GetZMoveToAtStart();
	UFUNCTION()
		float GetDistanceFromGround();
	UFUNCTION()
		void SetZMoveAtStart(bool newMoveAtStart);
	UFUNCTION()
		void SetStartZ(float newZ);

	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus stimulus);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnMainBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnMainBodyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		bool IsPositionReachable(FVector Position);

	UFUNCTION()
		void SetBlackboard(UBlackboardComponent* Blackboard);
	UFUNCTION()
		void SetBehaviourTree(UBehaviorTreeComponent* BehaviourTree);
};
