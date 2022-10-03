// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Components/SplineComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Kismet/GameplayStatics.h"
#include "PatrolPath.h"
#include "EnemyPatrolController.h"
#include "EnemyPatrol.h"
#include "BTTask_PatrolPath.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API UBTTask_PatrolPath : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_PatrolPath();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	int Index = 0;

	UPROPERTY(EditAnywhere)
	APatrolPath* PathRef;

	UPROPERTY(EditAnywhere)
	UBlackboardComponent* BBC;
	
};
