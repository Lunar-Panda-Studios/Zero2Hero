// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Reanimator.h"
#include "BTTask_RunAwayFromPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API UBTTask_RunAwayFromPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTTask_RunAwayFromPlayer();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	FBlackboardKeySelector BBKS;
};
