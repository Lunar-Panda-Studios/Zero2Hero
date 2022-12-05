// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SmoothRotate.h"

UBTTask_SmoothRotate::UBTTask_SmoothRotate()
{
    bCreateNodeInstance = true;
    NodeName = "SmoothRotate";
}

EBTNodeResult::Type UBTTask_SmoothRotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    //FMath::LerpRange(BBKS)

    UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

    AEnemy* Self = Cast<AEnemy>(BBC->GetValueAsObject("SelfActor"));
    AActor* Player = Cast<AActor>(BBC->GetValueAsObject(GetSelectedBlackboardKey()));

    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Self->GetActorLocation(), Player->GetActorLocation());

    LookAtRotation = FMath::LerpRange(Self->GetActorRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds() * RotationDelta);

    Self->SetActorRotation(FRotator(Self->GetActorRotation().Pitch, LookAtRotation.Yaw, Self->GetActorRotation().Roll));

    return EBTNodeResult::Succeeded;
}
