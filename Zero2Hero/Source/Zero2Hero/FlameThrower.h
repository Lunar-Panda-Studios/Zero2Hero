// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedWeapon.h"
#include "Enemy.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "FlameThrower.generated.h"
#include "FireBomb.h"

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
		float FBlaunchUP = 0.0f;
	UPROPERTY(EditAnywhere)
		float FBlaunchForward = 0.0f;
	
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* NiagaraComp;
	UPROPERTY(EditAnywhere)
		UNiagaraSystem* NigaraSys;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AFireBomb> FireBomb;		


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

};
