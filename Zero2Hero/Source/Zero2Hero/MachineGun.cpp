#include "MachineGun.h"

AMachineGun::AMachineGun()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMachineGun::BeginPlay()
{
	Super::BeginPlay();

	FireLocation = FindComponentByClass<USphereComponent>();
	WeaponType = 2;
}

void AMachineGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentCooldown += DeltaTime;
	Attack();
}

void AMachineGun::PrimaryAttack()
{
	shooting = true;
}

void AMachineGun::SecondaryAttack()
{
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
				FRotator rotation = GetActorRotation();
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
