// Fill out your copyright notice in the Description page of Project Settings.


#include "IceShotGun.h"

// Sets default values
AIceShotGun::AIceShotGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AIceShotGun::BeginPlay()
{
	Super::BeginPlay();

	FireLocation = FindComponentByClass<USphereComponent>();
	
}

// Called every frame
void AIceShotGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIceShotGun::PrimaryAttack()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Primary Attack"));

	if (FireLocation != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fire Location Set"));
		FRotator Rotation;

		FRotator Max = FRotator(GetActorRotation().Pitch, GetActorRotation().Roll + DegreesAroundCentre/2, GetActorRotation().Yaw);
		FRotator Min = FRotator(GetActorRotation().Pitch, GetActorRotation().Roll - DegreesAroundCentre/2, GetActorRotation().Yaw);

		if (DecreaseCharge(ChargeUsage))
		{
			for (int i = 0; i < IcicleNumber; i++)
			{
				Rotation = FRotator(GetActorRotation().Pitch, (GetActorRotation().Yaw - DegreesAroundCentre / 4) + FMath::FRandRange(Min.Yaw, Max.Yaw), (GetActorRotation().Roll - DegreesAroundCentre / 4) + FMath::FRandRange(Min.Roll, Max.Roll));
				AProjectile* Icicle = GetWorld()->SpawnActor<AProjectile>(Projectile, FireLocation->GetComponentLocation(), Rotation, spawnParams);
				if (Icicle != nullptr)
				{
					Icicle->Damage = Damage;
				}
			}
		}
	}
}

void AIceShotGun::SecondaryAttack()
{
}

