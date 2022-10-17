// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Projectile.h"
#include "ChargeBolt.generated.h"


/**
 * 
 */
UCLASS()
class ZERO2HERO_API AChargeBolt : public AProjectile
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* NiagaraComp;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* ElectricSystem;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ConnectionNiagaraSystem;

public:
	AChargeBolt();
	virtual void BeginPlay() override;
	void OnHit(AActor* OverlappedActor, AActor* OtherActor) override;

	UFUNCTION()
		void CheckArea(FVector pos);

	UFUNCTION()
		void OnParticleHit(AEnemy* Enemy);

	UFUNCTION()
		void ConnectCharge(AEnemy* Enemy);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float chargeRadius = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int maxEnemiesHit = 3;
	UPROPERTY()
		int currentEnemiesHit = 0;
};
