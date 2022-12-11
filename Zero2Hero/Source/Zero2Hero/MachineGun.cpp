#include "MachineGun.h"

AMachineGun::AMachineGun()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMachineGun::BeginPlay()
{
	Super::BeginPlay();
	WeaponTypeName = "Nature";
	FireLocation = FindComponentByClass<USphereComponent>();
	currentSecondaryCooldown = secondaryCooldown;
	WeaponType = 2;

	Manager = Cast<UGameManager>(GetWorld()->GetGameInstance());

	if (Manager != nullptr)
	{
		if (Manager->GetLoadingSave())
		{
			CurrentAmmo = Manager->GetNatureAmmo();
		}
	}
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
		rotation.Yaw += CameraAimDifferenceYaw;
		AActor* turretSeed = GetWorld()->SpawnActor<AActor>(SecondaryProjectile, FireLocation->GetComponentLocation(), rotation, spawnParams);
		ATurretSeed* seed = Cast<ATurretSeed>(turretSeed);
		seed->ammo = Charge;
		Charge = 0;
		seed->chargeUsage = ChargeUsage;
		currentSecondaryCooldown = 0;
		AmmoCheck();
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
		if (Camera != nullptr)
		{
			AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();

			FRotator Rotation = FRotator(Player->GetActorRotation().Pitch, Camera->GetSpringArm()->GetComponentRotation().Yaw, Player->GetActorRotation().Roll);
			Player->SetActorRotation(Rotation);

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
}
