#include "MachineGun.h"

AMachineGun::AMachineGun()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMachineGun::BeginPlay()
{
	Super::BeginPlay();

	FireLocation = FindComponentByClass<USphereComponent>();
	currentSecondaryCooldown = secondaryCooldown;
	WeaponType = 2;
}

void AMachineGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentCooldown += DeltaTime;
	currentSecondaryCooldown += DeltaTime;
	Attack();
}

void AMachineGun::PrimaryAttack()
{
	shooting = true;
}

void AMachineGun::SecondaryAttack()
{
	if (currentSecondaryCooldown > secondaryCooldown && DecreaseCharge(SecondaryChargeUsage))
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();
		FRotator rotation = Camera->GetSpringArm()->GetComponentRotation();
		rotation.Pitch += CameraAimDifference;
		AActor* turretSeed = GetWorld()->SpawnActor<AActor>(SecondaryProjectile, FireLocation->GetComponentLocation(), rotation, spawnParams);
		currentSecondaryCooldown = 0;
	}
}

void AMachineGun::PrimaryAttackEnd()
{
	shooting = false;
}

void AMachineGun::Attack()
{
	if (shooting)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();
		if (currentCooldown > fireRate)
		{
			if (DecreaseCharge(ChargeUsage))
			{
				FRotator rotation = Camera->GetSpringArm()->GetComponentRotation();
				rotation.Pitch += CameraAimDifference;
				AProjectile* Bullet = GetWorld()->SpawnActor<AProjectile>(Projectile, FireLocation->GetComponentLocation(), rotation, spawnParams);
				if (Bullet != nullptr)
				{
					Bullet->Damage = Damage;
				}
				currentCooldown = 0.0f;
			}
			
		}
	}
}
