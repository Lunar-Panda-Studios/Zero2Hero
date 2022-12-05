// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FlyTo.h"

UBTTask_FlyTo::UBTTask_FlyTo()
{
	bCreateNodeInstance = true;
	NodeName = "Fly To";

}

EBTNodeResult::Type UBTTask_FlyTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

	AEnemy* Self = Cast<AEnemy>(BBC->GetValueAsObject("SelfActor"));

	if (!Self->GetShouldMove())
	{
		return EBTNodeResult::Succeeded;
	}

	AActor* Actor;
	FVector newLocation;
	float Mag;

	if (GetSelectedBlackboardKey() == "PlayerActor")
	{
		Actor = Cast<AActor>(BBC->GetValueAsObject("PlayerActor"));

		if (Self->GetZMoveToAtStart())
		{
			newLocation = Self->GetActorLocation() - FVector(Self->GetActorLocation().X, Self->GetActorLocation().Y, Self->GetDistanceFromGround());
			Mag = newLocation.Size();
		}
		else
		{
			newLocation = FVector(Actor->GetActorLocation().X, Actor->GetActorLocation().Y, Actor->GetActorLocation().Z + ZOffsetFromGround) - FVector(Self->GetActorLocation().X, Self->GetActorLocation().Y, Self->GetActorLocation().Z);
			Mag = newLocation.Size();
		}

		if (Mag >= AcceptableDistance)
		{
			newLocation.Normalize();
			Self->LaunchCharacter(Self->GetMovementSpeed() * newLocation, true, true);
		}
		else
		{
			Self->GetMovementComponent()->Velocity = FVector(0, 0, 0);
		}
	}
	else
	{
		bool Zeroed = false;

		if (Self->GetZMoveToAtStart())
		{
			newLocation = FVector(Self->GetActorLocation().X, Self->GetActorLocation().Y, Self->GetDistanceFromGround()) - Self->GetActorLocation();
			Mag = newLocation.Size();

			if (Mag > AcceptableDistance)
			{

				BBC->SetValueAsBool("FlyToZ", true);
			}
			else
			{
				BBC->SetValueAsBool("FlyToZ", false);
				Self->GetMovementComponent()->Velocity = FVector(0, 0, 0);
				Zeroed = true;
				Self->SetZMoveAtStart(false);
				Self->SetStartZ(Self->GetActorLocation().Z);
			}
		}
		else
		{
			newLocation = BBC->GetValueAsVector(GetSelectedBlackboardKey());
			newLocation = (FVector(newLocation.X, newLocation.Y, newLocation.Z) - Self->GetActorLocation());
			
		}

		if (!Zeroed)
		{
			newLocation.Normalize();
			Self->LaunchCharacter(Self->GetMovementSpeed() * newLocation, true, true);
		}
	}

	return EBTNodeResult::Succeeded;
}
