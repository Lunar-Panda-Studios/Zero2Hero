// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingMissile.h"

// Sets default values
AHomingMissile::AHomingMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHomingMissile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMoveComp = FindComponentByClass<UProjectileMovementComponent>();
	//ProjectileMoveComp->HomingTargetComponent = GetWorld()->GetFirstPlayerController()->GetPawn()->GetRootComponent();

	Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	SetLifeSpan(0);
}

// Called every frame
void AHomingMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()));
	AddActorWorldTransform(FTransform(Speed * GetActorForwardVector()), true);

	if (isHoming)
	{
		TimerTilExplode += DeltaTime;

		if (TimerTilExplode >= TimeTillExplode)
		{
			Explode();
		}
	}
	else
	{
		//ForwardTimer += DeltaTime;
		//if (ForwardTimer >= TimeBeforeLockOn)
		//{
		//	ProjectileMoveComp->bIsHomingProjectile = true;
		//	isHoming = true;
		//	ForwardTimer = 0;
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Homing"));
		//}
	}
}

void AHomingMissile::Explode()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Destroy Missile Self"));
	HasDestruct = true;
	Destroy();
}

