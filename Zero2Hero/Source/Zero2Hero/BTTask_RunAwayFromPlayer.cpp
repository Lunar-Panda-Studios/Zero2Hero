// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RunAwayFromPlayer.h"

UBTTask_RunAwayFromPlayer::UBTTask_RunAwayFromPlayer()
{
	bCreateNodeInstance = true;
	NodeName = "Run from Player";
}

EBTNodeResult::Type UBTTask_RunAwayFromPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

	UWorld* World = GEngine->GameViewport->GetWorld();
	//FVector PlayerLocation = World->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	AActor* Player = World->GetFirstPlayerController()->GetPawn();
	ASummoner* Self = Cast<ASummoner>(BBC->GetValueAsObject("SelfActor"));

	FVector RandomLocation = Player->GetActorLocation() - Self->GetActorLocation();
	RandomLocation.Normalize();
	RandomLocation *= -1;

	RandomLocation.X *= Self->GetRunAwayDistance();
	RandomLocation.Y *= Self->GetRunAwayDistance();

	RandomLocation += Self->GetActorLocation();

	BBC->SetValue<UBlackboardKeyType_Vector>("TargetLocation", RandomLocation);

	return EBTNodeResult::Succeeded;
}
