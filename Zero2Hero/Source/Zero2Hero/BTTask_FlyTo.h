// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Enemy.h"
#include "BTTask_FlyTo.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API UBTTask_FlyTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTTask_FlyTo();

	UPROPERTY(EditAnywhere)
		float AcceptableDistance;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
