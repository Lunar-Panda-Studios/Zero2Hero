// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PerceptionCheck.h"

UBTTask_PerceptionCheck::UBTTask_PerceptionCheck()
{
	bCreateNodeInstance = true;
	NodeName = "Perception Check";
}

EBTNodeResult::Type UBTTask_PerceptionCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

	AEnemy* enemy = Cast<AEnemy>(BBC->GetValue<UBlackboardKeyType_Object>("SelfActor"));

	if (enemy != nullptr)
	{
		BBC->SetValue<UBlackboardKeyType_Bool>("LineOfSight", enemy->CanSee);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}