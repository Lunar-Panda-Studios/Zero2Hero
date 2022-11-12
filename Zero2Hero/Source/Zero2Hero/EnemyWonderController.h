// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyWonderController.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API AEnemyWonderController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		UBehaviorTree* BT;
	
};
