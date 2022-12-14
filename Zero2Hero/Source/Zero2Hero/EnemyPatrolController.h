// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolPath.h"
#include "EnemyPatrolController.generated.h"

/**
 * Help me
 */
UCLASS()
class ZERO2HERO_API AEnemyPatrolController : public AAIController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		UBehaviorTree* BT;

	void OnPossess(APawn* InPawn);
};
