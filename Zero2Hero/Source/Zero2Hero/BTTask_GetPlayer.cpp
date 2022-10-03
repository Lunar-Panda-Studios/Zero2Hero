// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetPlayer.h"

UBTTask_GetPlayer::UBTTask_GetPlayer()
{
	bCreateNodeInstance = true;
	NodeName = "Get Player Location";
}

EBTNodeResult::Type UBTTask_GetPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
	//BBC->ClearValue(BBKS.SelectedKeyName);

	UWorld* World = GEngine->GameViewport->GetWorld();
	FVector PlayerLocation = World->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	BBC->SetValue<UBlackboardKeyType_Vector>("PlayerLocation", PlayerLocation);

	return EBTNodeResult::Succeeded;
}
