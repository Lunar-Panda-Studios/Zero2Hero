// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyChaseController.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API AEnemyChaseController : public AAIController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		UBehaviorTree* BT;

	void OnPossess(APawn* InPawn);
	
};
