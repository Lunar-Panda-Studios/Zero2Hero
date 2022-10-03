// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPatrolController.h"

void AEnemyPatrolController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BT != nullptr)
	{
		RunBehaviorTree(BT);
	}
}
