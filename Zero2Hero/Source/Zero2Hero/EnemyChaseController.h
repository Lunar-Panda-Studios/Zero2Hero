// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Enemy.h"
#include "EnemyChaseController.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API AEnemyChaseController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyChaseController();

	UPROPERTY(EditAnywhere)
		UBehaviorTree* BT;

	void OnPossess(APawn* InPawn);

	//UFUNCTION()
	
};
