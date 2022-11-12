// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	SummonRangeMax = CreateDefaultSubobject<USphereComponent>(TEXT("SummonRangeMax"));
	SummonRangeMax->SetupAttachment(GetRootComponent());

	SummonRangeMin = CreateDefaultSubobject<USphereComponent>(TEXT("SummonRangeMin"));
	SummonRangeMin->SetupAttachment(GetRootComponent());

	FireLocationLeft = CreateDefaultSubobject<USphereComponent>(TEXT("FireLocationLeft"));
	FireLocationLeft->SetupAttachment(GetRootComponent());

	FireLocationRight = CreateDefaultSubobject<USphereComponent>(TEXT("FireLocationRight"));
	FireLocationRight->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ABoss::Tick(float DeltaTime)
{
	if (CurrentAttack == BossAttacks::Waiting)
	{
		if (BBC != nullptr)
		{
			BBC->SetValueAsInt("Phase", Phase);
		}

		switch (Phase)
		{
		case 0:
		{
			ChangePhase();
			break;
		}
		case 1:
		{
			break;
		}
		case 2:
		{
			Phase2AttackChoice();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("RandomChoice"));
			break;
		}
		default:
		{
			break;
		}
		}
	}

	switch (CurrentAttack)
	{
	case BossAttacks::P2RegularProjectile:
	{
		RegularProjectileFireTimer += DeltaTime;
		ProjectileAttack();
		break;
	}
	case BossAttacks::P2MissileProjectile:
	{
		MissileAttack();
		break;
	}
	case BossAttacks::P2SummonV1:
	{
		SummonType1();
		break;
	}
	case BossAttacks::P2SummonV2:
	{
		SummonType2();
		break;
	}
	default:
	{
		break;
	}
		
	}
}

void ABoss::SummonType1()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	FVector RandLocation;
	//FVector SummonRange = SummonRangeMax->GetScaledSphereRadius() - SummonRangeMin->GetScaledSphereRadius();
	float angle = FMath::Rand() * PI * 2;

	float x = FMath::Cos(angle) * SummonRangeMin->GetScaledSphereRadius();
	float y = FMath::Sin(angle) * SummonRangeMin->GetScaledSphereRadius();

	for (int i = 0; i < AmountToSummonV1; i++)
	{
		if (x > 0)
		{
			if (y > 0)
			{
				RandLocation = FVector(GetActorLocation().X + FMath::RandRange(x, SummonRangeMax->GetScaledSphereRadius()),
					GetActorLocation().Y + FMath::RandRange(y, SummonRangeMax->GetScaledSphereRadius()),
					SummonRangeMax->GetComponentLocation().Z);
			}
			else
			{
				RandLocation = FVector(GetActorLocation().X + FMath::RandRange(x, SummonRangeMax->GetScaledSphereRadius()),
					GetActorLocation().Y + FMath::RandRange(-SummonRangeMax->GetScaledSphereRadius(), y),
					SummonRangeMax->GetComponentLocation().Z);
			}
		}
		else
		{
			if (y > 0)
			{
				RandLocation = FVector(GetActorLocation().X + FMath::RandRange(-SummonRangeMax->GetScaledSphereRadius(), x),
					GetActorLocation().Y + FMath::RandRange(y, SummonRangeMax->GetScaledSphereRadius()),
					SummonRangeMax->GetComponentLocation().Z);
			}
			else
			{
				RandLocation = FVector(GetActorLocation().X + FMath::RandRange(-SummonRangeMax->GetScaledSphereRadius(), x),
					GetActorLocation().Y + FMath::RandRange(-SummonRangeMax->GetScaledSphereRadius(), y),
					SummonRangeMax->GetComponentLocation().Z);
			}
		}

		SummonedEnemies.Add(GetWorld()->SpawnActor<AEnemy>(Summon1EnemyTypeBP, RandLocation, GetActorRotation(), spawnParams));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned"));
	}

}

void ABoss::SummonType2()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	FVector RandLocation;
	//FVector SummonRange = SummonRangeMax->GetScaledSphereRadius() - SummonRangeMin->GetScaledSphereRadius();
	float angle = FMath::Rand() * PI * 2;

	float x = FMath::Cos(angle) * SummonRangeMin->GetScaledSphereRadius();
	float y = FMath::Sin(angle) * SummonRangeMin->GetScaledSphereRadius();

	for (int i = 0; i < AmountToSummonV1; i++)
	{
		if (x > 0)
		{
			if (y > 0)
			{
				RandLocation = FVector(GetActorLocation().X + FMath::RandRange(x, SummonRangeMax->GetScaledSphereRadius()),
					GetActorLocation().Y + FMath::RandRange(y, SummonRangeMax->GetScaledSphereRadius()),
					SummonRangeMax->GetComponentLocation().Z);
			}
			else
			{
				RandLocation = FVector(GetActorLocation().X + FMath::RandRange(x, SummonRangeMax->GetScaledSphereRadius()),
					GetActorLocation().Y + FMath::RandRange(-SummonRangeMax->GetScaledSphereRadius(), y),
					SummonRangeMax->GetComponentLocation().Z);
			}
		}
		else
		{
			if (y > 0)
			{
				RandLocation = FVector(GetActorLocation().X + FMath::RandRange(-SummonRangeMax->GetScaledSphereRadius(), x),
					GetActorLocation().Y + FMath::RandRange(y, SummonRangeMax->GetScaledSphereRadius()),
					SummonRangeMax->GetComponentLocation().Z);
			}
			else
			{
				RandLocation = FVector(GetActorLocation().X + FMath::RandRange(-SummonRangeMax->GetScaledSphereRadius(), x),
					GetActorLocation().Y + FMath::RandRange(-SummonRangeMax->GetScaledSphereRadius(), y),
					SummonRangeMax->GetComponentLocation().Z);
			}
		}

		SummonedEnemies.Add(GetWorld()->SpawnActor<AEnemy>(Summon2EnemyTypeBP, RandLocation, GetActorRotation(), spawnParams));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned"));
	}

}

void ABoss::MissileAttack()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	if (!HasFired)
	{
		AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FireLocationLeft->GetComponentLocation(), Player->GetActorLocation());

		Missile = GetWorld()->SpawnActor<AHomingMissile>(MissileBP, FireLocationRight->GetComponentLocation(), Rotation, spawnParams);
		HasFired = true;

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Missile Spawn"));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Has Fired"));
		if (Missile == nullptr)
		{
			HasFired = false;
			CurrentAttack = BossAttacks::Waiting;

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Destroy Pointer"));
		}
		else
		{
			if (Missile->GetHasDestruct())
			{
				HasFired = false;
				CurrentAttack = BossAttacks::Waiting;

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Destroy Fuction"));
			}
		}
	}
}

void ABoss::ProjectileAttack()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	if (RegularProjectileFireTimer >= RegularProjectileFireDelay)
	{
		AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FireLocationLeft->GetComponentLocation(), Player->GetActorLocation());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fire"));

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(RegularProjectileBP, FireLocationLeft->GetComponentLocation(), Rotation, spawnParams);
		RegularProjectileFireTimer = 0;
		AmountHasFired += 1;

		if (AmountHasFired >= AmountToFire)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Change"));
			AmountHasFired = 0;
			CurrentAttack = BossAttacks::Waiting;
		}
	}
}

void ABoss::ChangePhase()
{
	Phase += 1;
	if (BBC != nullptr)
	{
		BBC->SetValueAsInt("Phase", Phase);
	}
}

void ABoss::Phase2AttackChoice()
{
	switch (FMath::RandRange(1,1))
	{
	case 0:
	{
		CurrentAttack = BossAttacks::P2RegularProjectile;
		break;
	}
	case 1:
	{
		CurrentAttack = BossAttacks::P2MissileProjectile;
		break;
	}
	default:
		break;
	}
}

void ABoss::HarponSpawn()
{
	for (int i = 0; i < SummonedEnemies.Num(); i++)
	{
		if (SummonedEnemies[i] == nullptr)
		{
			SummonedEnemies.RemoveAt(i);
		}
	}

	if (SummonedEnemies.Num() == 1)
	{
		if (!HarponPiece1Spawned)
		{
			SummonedEnemies[0]->SetSpawnOnDeath(HarponPiece1);
		}
		else
		{
			SummonedEnemies[0]->SetSpawnOnDeath(HarponPiece2);
		}
	}
	else if (SummonedEnemies.Num() <= 0)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();

		if (!HarponPiece1Spawned)
		{
			GetWorld()->SpawnActor<AActor>(HarponPiece1, GetActorLocation(), GetActorRotation(), spawnParams);
		}
		else
		{
			GetWorld()->SpawnActor<AActor>(HarponPiece2, GetActorLocation(), GetActorRotation(), spawnParams);
		}
	}
}
	
