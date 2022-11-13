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

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	//Will need changing to Skeletal Mesh
	BodyMesh = FindComponentByClass<UStaticMeshComponent>();

	//Spawn Params
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	if (LeftHandCrystalBP != nullptr)
	{
		LeftHandCrystal = GetWorld()->SpawnActor<ABossCrystalWeakness>(LeftHandCrystalBP, GetActorLocation(), GetActorRotation(), spawnParams);
		AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandCrystalSocket);
	}

	if (RightHandCrystalBP != nullptr)
	{
		RightHandCrystal = GetWorld()->SpawnActor<ABossCrystalWeakness>(RightHandCrystalBP, GetActorLocation(), GetActorRotation(), spawnParams);
		AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandCrystalSocket);
	}
}

// Called when the game starts or when spawned
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
			if (SummonedEnemies.Num() == 0)
			{
				if (!HarponPiece1Spawned)
				{
					BossAttacks::P2SummonV1;
					SummonType1();
				}
				else
				{
					BossAttacks::P2SummonV2;
					SummonType2();
				}
			}
			Phase2AttackChoice();
			HarponSpawn();
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

FVector ABoss::CalculateSpawnLocation()
{
	FVector RandLocation;
	float Distance;

	do
	{
		RandLocation = FVector(GetActorLocation().X + FMath::RandRange(-SummonRangeMax->GetScaledSphereRadius(), SummonRangeMax->GetScaledSphereRadius()),
			GetActorLocation().Y + FMath::RandRange(-SummonRangeMax->GetScaledSphereRadius(), SummonRangeMax->GetScaledSphereRadius()),
			SummonRangeMax->GetComponentLocation().Z);

		FVector CompareLocation = SummonRangeMin->GetComponentLocation();
		CompareLocation.Z = RandLocation.Z;

		Distance = (CompareLocation - RandLocation).Size();

	} while (Distance < SummonRangeMin->GetScaledSphereRadius());

	return RandLocation;
}

void ABoss::Melee1Right()
{
	if (!GetMesh()->IsPlaying() && !HasPlayed)
	{
		GetMesh()->PlayAnimation(MeleeAttack1Right, false);
		HasPlayed = true;
	}

	if (!GetMesh()->IsPlaying() && HasPlayed)
	{
		switch (PhaseSection)
		{
		case 1:
		{
			CurrentAttack = BossAttacks::P1Melee2aL;
			break;
		}
		case 2:
		{
			CurrentAttack = BossAttacks::P1Melee1L;
		}
		default:
			break;
		}
	}
}

void ABoss::Melee1Left()
{
	if (!GetMesh()->IsPlaying() && !HasPlayed)
	{
		GetMesh()->PlayAnimation(MeleeAttack1Left, false);
		HasPlayed = true;
	}

	if (!GetMesh()->IsPlaying() && HasPlayed)
	{
		switch (PhaseSection)
		{
		case 1:
		{
			CurrentAttack = BossAttacks::P1Melee1R;
			break;
		}
		case 2:
		{
			CurrentAttack = BossAttacks::P1Melee2aR;
		}
		default:
			break;
		}
	}
}

void ABoss::Melee2aRight()
{
	if (!GetMesh()->IsPlaying() && !HasPlayed)
	{
		GetMesh()->PlayAnimation(MeleeAttack1aLeft, false);
		HasPlayed = true;
	}

	if (!GetMesh()->IsPlaying() && HasPlayed)
	{
		switch (PhaseSection)
		{
		case 1:
		{
			CurrentAttack = BossAttacks::P1Melee1R;
			break;
		}
		case 2:
		{
			CurrentAttack = BossAttacks::P1Melee2aL;
		}
		default:
			break;
		}
	}
}

void ABoss::Melee2aLeft()
{
	if (!GetMesh()->IsPlaying() && !HasPlayed)
	{
		GetMesh()->PlayAnimation(MeleeAttack1Left, false);
		HasPlayed = true;
	}

	if (!GetMesh()->IsPlaying() && HasPlayed)
	{
		CurrentAttack = BossAttacks::P1Melee2bR;
	}
}

void ABoss::Melee2bRight()
{
	if (!GetMesh()->IsPlaying() && !HasPlayed)
	{
		GetMesh()->PlayAnimation(MeleeAttack1bRight, false);
		HasPlayed = true;
	}

	if (!GetMesh()->IsPlaying() && HasPlayed)
	{
		CurrentAttack = BossAttacks::P1AoE1;
	}
}

void ABoss::Melee2bLeft()
{
	if (!GetMesh()->IsPlaying() && !HasPlayed)
	{
		GetMesh()->PlayAnimation(MeleeAttack1bLeft, false);
		HasPlayed = true;
	}
}

void ABoss::AoE1()
{
	if (AoE1SpawnCounter >= AoE1AmountToSpawn.Num())
	{
		if (AoE1DelayBetweenSpawns <= AoE1TimerBetweenSpawns)
		{
			PhaseSection = PhaseSection + 1 > 2 ? 1 : PhaseSection + 1;

			switch (PhaseSection)
			{
			case 1:
			{
				CurrentAttack = BossAttacks::P1Melee1L;
				break;
			}
			case 2:
			{
				CurrentAttack = BossAttacks::P1Melee1R;
			}
			default:
				break;
			}
		}
	}

	if (AoE1DelayBetweenSpawns <= AoE1TimerBetweenSpawns)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();

		FVector SpawnLocation;

		for (int i = 0; i < AoE1AmountToSpawn[AoE1SpawnCounter]; i++)
		{
			SpawnLocation = CalculateSpawnLocation();
			SpawnLocation.Z += AoE1ZOffset;

			FallingItems.Add(GetWorld()->SpawnActor<AFallingItem>(FallingItemBP, SpawnLocation, GetActorRotation(), spawnParams));
		}

		AoE1TimerBetweenSpawns = 0;
		AoE1SpawnCounter = AoE1SpawnCounter + 1;
	}
}

void ABoss::SummonType1()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	FVector RandLocation;

	for (int i = 0; i < AmountToSummonV1; i++)
	{
		RandLocation = CalculateSpawnLocation();
		RandLocation.Z += ZSummonOffSet;

		//I'm about to tank the framerate lmao
		//Very sad
		//But if people don't know about it then I can't be asked to fix it
		SummonedEnemies.Add(GetWorld()->SpawnActor<AEnemy>(Summon1EnemyTypeBP, RandLocation, GetActorRotation(), spawnParams));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned"));
	}

}

void ABoss::SummonType2()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	FVector RandLocation;

	for (int i = 0; i < AmountToSummonV1; i++)
	{
		RandLocation = CalculateSpawnLocation();
		RandLocation.Z += ZSummonOffSet;

		//Ah shit here we go again
		//Plz send help
		SummonedEnemies.Add(GetWorld()->SpawnActor<AEnemy>(Summon2EnemyTypeBP, RandLocation, GetActorRotation(), spawnParams));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned"));
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

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Missile Spawn"));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Has Fired"));
		if (Missile == nullptr)
		{
			HasFired = false;
			CurrentAttack = BossAttacks::Waiting;

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Destroy Pointer"));
		}
		else
		{
			if (Missile->GetHasDestruct())
			{
				HasFired = false;
				CurrentAttack = BossAttacks::Waiting;

				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Destroy Fuction"));
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
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fire"));

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(RegularProjectileBP, FireLocationLeft->GetComponentLocation(), Rotation, spawnParams);
		RegularProjectileFireTimer = 0;
		AmountHasFired += 1;

		if (AmountHasFired >= AmountToFire)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Change"));
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
	switch (FMath::RandRange(0,1))
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
			if (HarponPiece1 != nullptr)
			{
				GetWorld()->SpawnActor<AActor>(HarponPiece1, GetActorLocation(), GetActorRotation(), spawnParams);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Harpon 1 not set"));
			}
		}
		else
		{
			if (HarponPiece1 != nullptr)
			{
				GetWorld()->SpawnActor<AActor>(HarponPiece2, GetActorLocation(), GetActorRotation(), spawnParams);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Harpon 2 not set"));
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Kill more enemies"));
	}
}

void ABoss::OnHitArms(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!ShouldDamage)
	{
		return;
	}

	if (OtherActor == nullptr)
	{
		return;
	}

	if (OtherActor->ActorHasTag("Player"))
	{
		if (OtherComp == nullptr)
		{
			return;
		}

		if (OtherComp->ComponentHasTag("Ignore"))
		{
			return;
		}
	}
}
	
