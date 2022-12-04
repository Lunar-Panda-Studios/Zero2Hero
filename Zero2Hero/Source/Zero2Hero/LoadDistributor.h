// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.h"
#include "PlayerCharacter.h"
#include "LoadDistributor.generated.h"

UCLASS()
class ZERO2HERO_API ALoadDistributor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALoadDistributor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		UGameManager* Manager;
	UPROPERTY()
		APlayerCharacter* Player;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		bool DistributeLoad();

	UFUNCTION(BlueprintCallable)
		FPowerCoreLocation DistributePowerCore(FName Colour);
	UFUNCTION(BlueprintCallable)
		bool DistributeBridgeStatus();
	UFUNCTION()
		bool DistributePlayerLocation();
	UFUNCTION()
		bool DistributeAmmo();

};
