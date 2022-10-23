#include "ChargeRifle.h"

void AChargeRifle::BeginPlay()
{
	Super::BeginPlay();
	FireLocation = FindComponentByClass<USphereComponent>();
}

void AChargeRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentCooldown += DeltaTime;
	Attack();
}

AChargeRifle::AChargeRifle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AChargeRifle::PrimaryAttack()
{
	shooting = true;
}

void AChargeRifle::SecondaryAttack()
{
}

void AChargeRifle::PrimaryAttackEnd()
{
	shooting = false;
}

void AChargeRifle::Attack()
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
				AProjectile* ChargeBolt = GetWorld()->SpawnActor<AProjectile>(Projectile, FireLocation->GetComponentLocation(), rotation, spawnParams);
				if (ChargeBolt != nullptr)
				{
					ChargeBolt->Damage = Damage;
				}
				currentCooldown = 0.0f;
			}

		}
	}
}
