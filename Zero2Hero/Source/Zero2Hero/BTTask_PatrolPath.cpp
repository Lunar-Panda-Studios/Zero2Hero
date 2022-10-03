// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PatrolPath.h"

UBTTask_PatrolPath::UBTTask_PatrolPath()
{
    bCreateNodeInstance = true;
    NodeName = "Patrol Path";
}

EBTNodeResult::Type UBTTask_PatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    BBC = OwnerComp.GetBlackboardComponent();

    AEnemyPatrol* enemy = Cast<AEnemyPatrol>(BBC->GetValueAsObject("SelfActor"));

    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), BBC->GetOwner()));

    if (enemy == nullptr)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Failed No enemy Ref"));
        return EBTNodeResult::Failed;
    }

    PathRef = enemy->PathRef;

    if (PathRef == nullptr)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Failed No path Ref"));
        return EBTNodeResult::Failed;
    }

    if (PathRef->Locations.Num() < 1)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Failed Not long enough"));
        return EBTNodeResult::Failed;
    }

    OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("PatrolLocation", PathRef->Locations[Index]);

    if (Index < PathRef->Locations.Num() - 1)
    {
        Index++;
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Success"));
        return EBTNodeResult::Succeeded;
    }

    Index = 0;
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Success"));
    return EBTNodeResult::Succeeded;
}
