// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyChaseController.h"

void AEnemyChaseController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BT != nullptr)
	{
		RunBehaviorTree(BT);
	}
}