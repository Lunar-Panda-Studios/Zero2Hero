// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "Engine/Engine.h"
#include "EventZone.generated.h"

UCLASS()
class ZERO2HERO_API AEventZone : public AActor
{
	GENERATED_BODY()
public:
	AEventZone();
	~AEventZone();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void CheckEnemies();

	UFUNCTION()
		void RemoveEnemy(AEnemy* enemy);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	
};
