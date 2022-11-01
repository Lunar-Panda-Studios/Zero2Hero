// Fill out your copyright notice in the Description page of Project Settings.
#include "Turret.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Damageable.h"
#include "Enemy.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ignoreActors.Init(this, 1);
	seekClass = AEnemy::StaticClass();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fireRateCooldown += DeltaTime;

	if (fireRateCooldown > turretFireRate)
	{
		actors.Empty();
		if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), turretRange, traceObjectTypes, seekClass, ignoreActors, actors))
		{

			AActor* closestActor{};
			float currentDist = 0.0f;
			for (AActor* a : actors)
			{
				
				FVector LineTraceEnd = FVector(a->GetActorLocation());
				FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
				TraceParams.AddIgnoredActor(this);

				FHitResult Hit;
				if (GetWorld()->LineTraceSingleByChannel(OUT Hit, GetActorLocation(), LineTraceEnd, ECollisionChannel::ECC_Pawn, TraceParams, FCollisionResponseParams()))
				{
					if (Cast<AEnemy>(Hit.GetActor()))
					{
						if (closestActor != nullptr)
						{
							GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, a->GetName());
							float thisDist = FVector::Dist(a->GetActorLocation(), GetActorLocation());
							if (thisDist < currentDist)
							{
								closestActor = a;
								currentDist = thisDist;
							}

						}
						else
						{
							closestActor = a;
							currentDist = FVector::Dist(closestActor->GetActorLocation(), GetActorLocation());
						}
					}
				}
			}
			if (closestActor != nullptr)
			{
				
				SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), closestActor->GetActorLocation()));
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;
				spawnParams.Instigator = GetInstigator();
				FRotator rotation = GetActorRotation();
				AProjectile* Bullet = GetWorld()->SpawnActor<AProjectile>(projectile, GetActorLocation(), rotation, spawnParams);
				if (Bullet != nullptr)
				{
					Bullet->Damage = damage;
				}
				fireRateCooldown = 0.0f;
				--turretAmmo;
				if (turretAmmo < 1)
				{
					Destroy();
				}
			}
			
		}
	}
}

