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

	if (FireLocation != nullptr)
	{
		FRotator Rotation = GetActorRotation();
		AProjectile* Icicle = GetWorld()->SpawnActor<AProjectile>(Projectile, FireLocation->GetComponentLocation(), Rotation, spawnParams);
		
		Rotation = FRotator(Rotation.Pitch, Rotation.Yaw + DegreesFromCentre, Rotation.Roll);
		Icicle = GetWorld()->SpawnActor<AProjectile>(Projectile, FireLocation->GetComponentLocation(), Rotation, spawnParams);

		Rotation = FRotator(Rotation.Pitch, Rotation.Yaw - (DegreesFromCentre*2), Rotation.Roll);
		Icicle = GetWorld()->SpawnActor<AProjectile>(Projectile, FireLocation->GetComponentLocation(), Rotation, spawnParams);
	}
}

void AIceShotGun::SecondaryAttack()
{
}

