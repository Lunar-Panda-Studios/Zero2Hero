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

	WeaponTypeName = "Ice";
	FireLocation = FindComponentByClass<USphereComponent>();
	WeaponType = 1;
	
}

// Called every frame
void AIceShotGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CanShoot)
	{
		CanShootTimer += DeltaTime;

		if (CanShootTimer >= CanShootDelay)
		{
			CanShoot = true;
			CanShootTimer = 0;
		}
	}

}

void AIceShotGun::PrimaryAttack()
{
	if (CanShoot)
	{
		if (Camera != nullptr)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = GetInstigator();
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Primary Attack"));

			if (FireLocation != nullptr)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fire Location Set"));
				FRotator Rotation;

				FRotator Temp = Camera->GetSpringArm()->GetComponentRotation();
				Temp.Pitch += CameraAimDifference;

				FRotator Max = FRotator(Temp.Pitch + DegreesAroundCentre / 2, Temp.Roll + DegreesAroundCentre / 2, Temp.Roll);
				FRotator Min = FRotator(Temp.Pitch - DegreesAroundCentre / 2, Temp.Roll - DegreesAroundCentre / 2, Temp.Roll);

				if (DecreaseCharge(ChargeUsage))
				{
					for (int i = 0; i < IcicleNumber; i++)
					{
						Rotation = FRotator((Temp.Pitch - DegreesAroundCentre / IcicleNumber) + FMath::FRandRange(Min.Pitch, Max.Pitch), (Temp.Yaw - DegreesAroundCentre / IcicleNumber) + FMath::FRandRange(Min.Yaw, Max.Yaw), (Temp.Roll - DegreesAroundCentre / 4) + FMath::FRandRange(Min.Roll, Max.Roll));
						AProjectile* Icicle = GetWorld()->SpawnActor<AProjectile>(Projectile, FireLocation->GetComponentLocation(), Rotation, spawnParams);
						if (Icicle != nullptr)
						{
							Icicle->Damage = Damage;
						}
					}
				}
			}

			CanShoot = false;
		}
	}
}

void AIceShotGun::SecondaryAttack()
{
}

