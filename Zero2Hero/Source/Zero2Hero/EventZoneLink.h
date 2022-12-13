// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "EventZoneLink.generated.h"

UCLASS()
class ZERO2HERO_API AEventZoneLink : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEventZoneLink();

	UPROPERTY(BlueprintReadWrite)
		TArray<AEnemy*> enemies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float radius = 1000.0f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	UPROPERTY()
		TArray<AActor*> ignoreActors;
	UPROPERTY()
		TArray<AActor*> actors;
	UPROPERTY()
		UClass* seekClass;
	UPROPERTY()
		UGameManager* Manager;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		void AllEnemiesKilled();
	UFUNCTION()
		TArray<AEnemy*> GetEnemies();
	UFUNCTION(BlueprintCallable)
		void AutoKill();

};
