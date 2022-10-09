// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "Enemy.h"
#include "BTTask_PerceptionCheck.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API UBTTask_PerceptionCheck : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	UBTTask_PerceptionCheck();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};
