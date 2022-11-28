#include "ChargeRifle.h"
#include "SuctionGrenade.h"


void AChargeRifle::BeginPlay()
{
	Super::BeginPlay();
	WeaponTypeName = "Electric";
	FireLocation = FindComponentByClass<USphereComponent>();
	WeaponType = 3;
	secondaryCurrentCooldown = secondaryFireRate;

	Manager = Cast<UGameManager>(GetWorld()->GetGameInstance());

	if (Manager != nullptr)
	{
		if (Manager->GetLoadingSave())
		{
			CurrentAmmo = Manager->GetElectricAmmo();
		}
	}
}

void AChargeRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentCooldown += DeltaTime;
	
	secondaryCurrentCooldown += DeltaTime;
	Attack(DeltaTime);
}

AChargeRifle::AChargeRifle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AChargeRifle::PrimaryAttack()
{
	if (currentCooldown > fireRate && DecreaseCharge(ChargeUsage))
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();

		FRotator rotation = Camera->GetSpringArm()->GetComponentRotation();
		rotation.Pitch += CameraAimDifference;
		rotation.Yaw += CameraAimDifferenceYaw;

		ASuctionGrenade* succ = GetWorld()->SpawnActor<ASuctionGrenade>(SecondaryProjectile, FireLocation->GetComponentLocation(), rotation, spawnParams);
		/*UStaticMeshComponent* meshComp = succ->FindComponentByClass<UStaticMeshComponent>();*/
		USphereComponent* sphereCol = succ->FindComponentByClass<USphereComponent>();
		if (sphereCol && succ)
		{
			sphereCol->AddImpulse(GetActorForwardVector() * secondaryLaunchSpeed);/*
			meshComp->AddImpulse(GetActorForwardVector() * secondaryLaunchSpeed);*/
		}
		currentCooldown = 0.0f;

	}
	/*shooting = true;*/
}

void AChargeRifle::SecondaryAttack()
{
	
}

void AChargeRifle::PrimaryAttackEnd()
{
	/*shooting = false;
	currentFireTime = 0.0f;
	hasFired = false;
	StopCharging();*/
}

void AChargeRifle::Attack(float DeltaTime)
{
	/*if (shooting)
	{
		currentFireTime += DeltaTime;
		if (!hasFired && currentCooldown > fireRate - fireTime)
		{
			OnFire();
			hasFired = true;
		}
		
		if (currentCooldown > fireRate && currentFireTime > fireTime)
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
				currentFireTime = 0.0f;
				hasFired = false;
			}

		}
	}*/
}
