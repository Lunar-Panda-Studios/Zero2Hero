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

	FVector newLocation;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(Self);

	FHitResult Hit;

	int repeated = 0;
	int MaxRepeated = 10;

	bool Valid = false;

	do
	{
		Valid = false;

		repeated += 1;

		float ZPosition = Self->GetDistanceFromGround() != 0 ? Self->GetDistanceFromGround() + FMath::RandRange(-Self->GetZRadius(), Self->GetZRadius()) : Self->GetStartLocation().Z;

		newLocation = FVector(FMath::RandRange(Self->GetStartLocation().X - Self->GetXRadius(), Self->GetStartLocation().X + Self->GetXRadius()),
			FMath::RandRange(Self->GetStartLocation().Y - Self->GetYRadius(), Self->GetStartLocation().Y + Self->GetYRadius()),
			ZPosition);

		GetWorld()->LineTraceSingleByChannel(OUT Hit, Self->GetActorLocation(), newLocation, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams());
		//DrawDebugLine(GetWorld(), Self->GetActorLocation(), newLocation, FColor::Black, false, 1.0f, 0, 5);

		if (Hit.IsValidBlockingHit())
		{
			Valid = false;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Hit.GetActor()->GetFName().ToString());
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Valid"));
			Valid = true;
		}

	} while ((repeated < MaxRepeated) && !Valid);
	
	if (repeated != MaxRepeated)
	{
		BBC->SetValueAsVector("TargetLocation", newLocation);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
