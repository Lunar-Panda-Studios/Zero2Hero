// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "BossCrystalWeakness.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API ABossCrystalWeakness : public ADamageable
{
	GENERATED_BODY()

	void BeginPlay();

	UPROPERTY()
		UStaticMeshComponent* MeshBody;

	UPROPERTY(EditAnywhere)
		bool isRightHandCrystal;

	UFUNCTION()
		void OnCrystalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
