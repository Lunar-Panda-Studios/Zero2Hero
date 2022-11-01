// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRandomLocation.h"

UBTTask_GetRandomLocation::UBTTask_GetRandomLocation()
{
	bCreateNodeInstance = true;
	NodeName = "Get Random Location";
}

EBTNodeResult::Type UBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

	AEnemyWondering* Self = Cast<AEnemyWondering>(BBC->GetValueAsObject("SelfActor"));

	if (BBC->GetValueAsFloat("XRadius") == 0 && BBC->GetValueAsFloat("YRadius") == 0)
	{
		BBC->SetValueAsFloat("XRadius", Self->GetXRadius());
		BBC->SetValueAsFloat("YRadius", Self->GetYRadius());
	}

	FVector newLocation;

	do
	{
		newLocation = FVector(FMath::RandRange(Self->GetStartLocation().X - BBC->GetValueAsFloat("XRadius"), Self->GetStartLocation().X + BBC->GetValueAsFloat("XRadius")),
			FMath::RandRange(Self->GetStartLocation().Y - BBC->GetValueAsFloat("YRadius"), Self->GetStartLocation().Y + BBC->GetValueAsFloat("YRadius")),
			Self->GetStartLocation().Z);

	} while (!Self->IsPositionReachable(newLocation));
	
	BBC->SetValueAsVector("TargetLocation", newLocation);

	return EBTNodeResult::Succeeded;
}
