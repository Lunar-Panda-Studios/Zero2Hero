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

	enemy->SetBehaviourTree(&OwnerComp);
	enemy->SetBlackboard(BBC);

	if (enemy != nullptr)
	{
		if (!enemy->GetDisenage())
		{
			BBC->SetValue<UBlackboardKeyType_Bool>("LineOfSight", enemy->GetCanSee());
		}
		else
		{
			BBC->SetValue<UBlackboardKeyType_Bool>("LineOfSight", false);
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}