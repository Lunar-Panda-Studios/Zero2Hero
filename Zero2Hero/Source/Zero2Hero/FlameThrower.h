// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedWeapon.h"
#include "Enemy.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "FireBomb.h"
#include "FlameThrower.generated.h"

UCLASS()
class ZERO2HERO_API AFlameThrower : public ARangedWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlameThrower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		bool InUse = false;
	UPROPERTY()
		float DamageTimer = 0.0f;
	UPROPERTY()
		float DamageTimerMax = 1.0f;
	UPROPERTY()
		float Timer = 0.0f;
	UPROPERTY()
		float TimerMax = 1.0f;
	UPROPERTY(EditAnywhere)
		float launchSpeed = 500.0f;
	
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* NiagaraComp;
	UPROPERTY(EditAnywhere)
		UNiagaraSystem* NigaraSys;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AFireBomb> FireBomb;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float timeToThrow = 0.1f;
	UPROPERTY()
		float currentTimeToThrow = 0.0f;
	UPROPERTY()
		bool shoot = false;
	UPROPERTY()
		float zLaunchRot = 20.0f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void PrimaryAttack() override;
	UFUNCTION()
		void PrimaryAttackEnd();
	
	UFUNCTION()
		void OnParticleHit(AEnemy* Enemy);

	UFUNCTION()
		void SecondaryAttack() override;

	UFUNCTION()
		void Fire();

};
