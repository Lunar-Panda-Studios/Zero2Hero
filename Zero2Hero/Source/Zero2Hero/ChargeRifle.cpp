#include "ChargeRifle.h"
#include "SuctionGrenade.h"


void AChargeRifle::BeginPlay()
{
	Super::BeginPlay();
	FireLocation = FindComponentByClass<USphereComponent>();
	WeaponType = 3;
	secondaryCurrentCooldown = secondaryFireRate;
}

void AChargeRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentCooldown += DeltaTime;
	secondaryCurrentCooldown += DeltaTime;
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
	if (secondaryCurrentCooldown > secondaryFireRate && DecreaseCharge(SecondaryChargeUsage))
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();

		FRotator rotation = Camera->GetSpringArm()->GetComponentRotation();
		rotation.Pitch += CameraAimDifference;

		ASuctionGrenade* succ = GetWorld()->SpawnActor<ASuctionGrenade>(SecondaryProjectile, FireLocation->GetComponentLocation(), rotation, spawnParams);
		/*UStaticMeshComponent* meshComp = succ->FindComponentByClass<UStaticMeshComponent>();*/
		USphereComponent* sphereCol = succ->FindComponentByClass<USphereComponent>();
		if (sphereCol && succ)
		{
			sphereCol->AddImpulse(GetActorForwardVector() * secondaryLaunchSpeed);/*
			meshComp->AddImpulse(GetActorForwardVector() * secondaryLaunchSpeed);*/
		}
		secondaryCurrentCooldown = 0.0f;
		
	}
}

void AChargeRifle::PrimaryAttackEnd()
{
	shooting = false;
}

void AChargeRifle::Attack()
{
	if (shooting)
	{
		
		if (currentCooldown > fireRate)
		{
			if (DecreaseCharge(ChargeUsage))
			{
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;
				spawnParams.Instigator = GetInstigator();
				FRotator rotation = Camera->GetSpringArm()->GetComponentRotation();
				rotation.Pitch += CameraAimDifference;
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
