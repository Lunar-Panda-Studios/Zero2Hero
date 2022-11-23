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

	if (BBC != nullptr)
	{
		BBC->SetValueAsInt("Phase", Phase);
	}
}

// Called when the game starts or when spawned
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->InputEnabled())
	{
		if (BBC != nullptr)
		{
			BBC->SetValueAsInt("Phase", Phase);
		}

		if (CurrentAttack == BossAttacks::Waiting)
		{
			switch (Phase)
			{
			case 0:
			{
				ChangePhase();
				break;
			}
			case 1:
			{
				CurrentAttack = BossAttacks::P1Melee1L;
				break;
			}
			case 2:
			{
				if (SummonedEnemies.Num() == 0 && ReadyToSpawn)
				{
					ReadyToSpawn = false;
					if ((!Launcher1Fixed || !Launcher2Fixed) /*&& (Harpoon2Launched && Harpoon1Launched)*/ ||
						((Launcher1Fixed && Harpoon1Launched) && !Launcher2Fixed) ||
						(Launcher2Fixed && Harpoon2Launched) && !Launcher1Fixed)
					{
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Spawn 1"));
						CurrentAttack = BossAttacks::P2SummonV1;
						SummonType1();
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Spawn 2"));
						CurrentAttack = BossAttacks::P2SummonV2;
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
			//Phase 1 Attacks
		case BossAttacks::P1Melee1L:
		{

			Melee1Left();
			ShouldEndPhase1();
			break;
		}
		case BossAttacks::P1Melee1R:
		{
			Melee1Right();
			ShouldEndPhase1();
			break;
		}
		case BossAttacks::P1Melee2aL:
		{
			if (FirstAnimFinished)
			{
				MeleeAttack2aLeftTimer += DeltaTime;
			}
			Melee2aLeft();
			ShouldEndPhase1();
			break;
		}
		case BossAttacks::P1Melee2aR:
		{
			if (FirstAnimFinished)
			{
				MeleeAttack2aRightTimer += DeltaTime;
			}
			Melee2aRight();
			ShouldEndPhase1();
			break;
		}
		case BossAttacks::P1Melee2bL:
		{
			if (FirstAnimFinished)
			{
				MeleeAttack2bLeftTimer += DeltaTime;
			}
			Melee2bLeft();
			ShouldEndPhase1();
			break;
		}
		case BossAttacks::P1Melee2bR:
		{
			if (FirstAnimFinished)
			{
				MeleeAttack2bRightTimer += DeltaTime;
			}
			Melee2bRight();
			ShouldEndPhase1();
			break;
		}
		case BossAttacks::P1AoE1:
		{
			AoE1TimerBetweenSpawns += DeltaTime;
			AoE1();
			ShouldEndPhase1();
			break;
		}

		//Phase 2 Attacks
		case BossAttacks::P2RegularProjectile:
		{
			RegularProjectileFireTimer += DeltaTime;
			ProjectileAttack();
			break;
		}
		case BossAttacks::P2MissileProjectile:
		{
			if (HasFired)
			{
				TimerToNext += DeltaTime;
			}
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
}

FVector ABoss::CalculateSpawnLocation()
{
	FVector RandLocation;
	float Distance;

	float MaxRange = SummonRangeMax->GetScaledSphereRadius();
	float MinRange = SummonRangeMin->GetScaledSphereRadius();

	do
	{
		RandLocation = FVector(GetActorLocation().X + FMath::RandRange(-MaxRange, MaxRange),
			GetActorLocation().Y + FMath::RandRange(-MaxRange, MaxRange),
			SummonRangeMax->GetComponentLocation().Z);

		FVector CompareLocation = SummonRangeMin->GetComponentLocation();
		CompareLocation.Z = RandLocation.Z;

		Distance = (CompareLocation - RandLocation).Size();

	} while (Distance < MinRange || Distance > MaxRange);

	return RandLocation;
}

void ABoss::Melee1Right()
{
	if (!GetMesh()->IsPlaying() && !HasPlayed)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Right Sweep Begin"));
		ShouldDamage = true;
		GetMesh()->PlayAnimation(MeleeAttack1RightTo, false);
		HasPlayed = true;

		if (BBC != nullptr)
		{
			BBC->SetValueAsInt("IsAttacking", true);
		}
	}

	if (!GetMesh()->IsPlaying() && HasPlayed)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Right Sweep End"));
		HasPlayed = false;
		ShouldDamage = false;
		if (HandsAlive() == 2)
		{
			switch (PhaseSection)
			{
			case 1:
			{
				
				CurrentAttack = CurrentAttack == BossAttacks::Waiting? BossAttacks::Waiting: BossAttacks::P1Melee2aL;
				break;
			}
			case 2:
			{
				CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee1L;
			}
			default:
				break;
			}
		}
		else
		{
			CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee2aR;
		}

		if (BBC != nullptr)
		{
			BBC->SetValueAsInt("IsAttacking", false);
		}
	}
}

void ABoss::Melee1Left()
{
	if (!GetMesh()->IsPlaying() && !HasPlayed)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Left Sweep Begin"));
		ShouldDamage = true;
		GetMesh()->PlayAnimation(MeleeAttack1LeftTo, false);
		HasPlayed = true;

		if (BBC != nullptr)
		{
			BBC->SetValueAsInt("IsAttacking", true);
		}
	}

	if (!GetMesh()->IsPlaying() && HasPlayed)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Left Sweep End"));
		HasPlayed = false;
		ShouldDamage = true;
		if (HandsAlive() == 2)
		{
			switch (PhaseSection)
			{
			case 1:
			{
				CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee1R;
				break;
			}
			case 2:
			{
				CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee2aR;
			}
			default:
				break;
			}
		}
		else
		{
			CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee2aL;
		}

		if (BBC != nullptr)
		{
			BBC->SetValueAsInt("IsAttacking", false);
		}
	}
}

void ABoss::Melee2aRight()
{
	if (!GetMesh()->IsPlaying() && !HasPlayed)
	{
		ShouldDamage = true;
		GetMesh()->PlayAnimation(MeleeAttack2aRightTo, false);
		HasPlayed = true;

		if (BBC != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Right Fist Start"));
			BBC->SetValueAsInt("IsAttacking", true);
		}
	}

	if (!GetMesh()->IsPlaying() && HasPlayed)
	{
		FirstAnimFinished = true;
		HasHandHitGround = true;
		ShouldDamage = false;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Right Fist Ground"));

		if (MeleeAttack2aRightTimeDown <= MeleeAttack2aRightTimer)
		{
			if (!GetMesh()->IsPlaying() && FirstAnimFinished && !SecondAnimStarted)
			{
				GetMesh()->PlayAnimation(MeleeAttack2bRightReturn, false);
				SecondAnimStarted = true;
			}

			if (!GetMesh()->IsPlaying() && SecondAnimStarted)
			{
				MeleeAttack2aRightTimer = 0;
				HasPlayed = false;
				HasHandHitGround = false;

				if (HandsAlive() == 2)
				{
					CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee2bL;
				}
				else
				{
					CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee2bR;
				}

				if (BBC != nullptr)
				{
					BBC->SetValueAsInt("IsAttacking", false);
				}
			}
		}
	}
}

void ABoss::ShockWave()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	ShockWaveInstance = GetWorld()->SpawnActor<AShockWave>(ShockWaveBP, HandDownLocation, GetActorRotation(), spawnParams);
}

void ABoss::Melee2aLeft()
{
	if (!GetMesh()->IsPlaying() && !HasPlayed)
	{
		ShouldDamage = true;
		GetMesh()->PlayAnimation(MeleeAttack2aLeftTo, false);
		HasPlayed = true;

		if (BBC != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Left Fist Begin"));
			BBC->SetValueAsInt("IsAttacking", true);
		}
	}

	if (!GetMesh()->IsPlaying() && HasPlayed)
	{
		FirstAnimFinished = true;
		ShouldDamage = false;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Left Fist End"));
		if (MeleeAttack2aLeftTimeDown <= MeleeAttack2aLeftTimer)
		{
			if (!GetMesh()->IsPlaying() && FirstAnimFinished && !SecondAnimStarted)
			{
				GetMesh()->PlayAnimation(MeleeAttack2aLeftReturn, false);
				SecondAnimStarted = true;
			}

			if (!GetMesh()->IsPlaying() && SecondAnimStarted)
			{
				FirstAnimFinished = false;
				SecondAnimStarted = false;
				MeleeAttack2aLeftTimer = 0;
				HasPlayed = false;
				if (HandsAlive() == 2)
				{
					CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee2bR;
				}
				else
				{
					CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee2bL;
				}

				if (BBC != nullptr)
				{
					BBC->SetValueAsInt("IsAttacking", false);
				}
			}
		}
	}
}

void ABoss::CalculateHandLocation()
{
	AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	HandDownLocation = Player->GetActorLocation();

	FVector LineTraceEnd = FVector(Player->GetActorLocation().X, Player->GetActorLocation().Y, Player->GetActorLocation().Z - DistanceTrace);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(Player);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(OUT Hit, Player->GetActorLocation(), LineTraceEnd, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams());
	//DrawDebugLine(GetWorld(), GetActorLocation(), LineTraceEnd, FColor::Blue, false, 5.0f, 0, 5);

	if (Hit.IsValidBlockingHit())
	{
		HandDownLocation.Z = Hit.ImpactPoint.Z;
	}
}

void ABoss::Melee2bRight()
{
	if (!GetMesh()->IsPlaying() && !HasPlayed)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Right Slap Begin"));
		ShouldDamage = true;
		CalculateHandLocation();
		GetMesh()->PlayAnimation(MeleeAttack2bRightTo, false);
		HasPlayed = true;

		if (BBC != nullptr)
		{
			BBC->SetValueAsInt("IsAttacking", true);
		}
	}

	if (!GetMesh()->IsPlaying() && HasPlayed)
	{
		FirstAnimFinished = true;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Right Slap End"));
		ShockWave();
		ShouldDamage = false;

		if (MeleeAttack2bRightTimeDown <= MeleeAttack2bRightTimer)
		{
			if (!GetMesh()->IsPlaying() && FirstAnimFinished && !SecondAnimStarted)
			{
				GetMesh()->PlayAnimation(MeleeAttack2bRightReturn, false);
				SecondAnimStarted = true;
			}

			if (!GetMesh()->IsPlaying() && SecondAnimStarted)
			{
				FirstAnimFinished = false;
				SecondAnimStarted = false;
				MeleeAttack2bRightTimer = 0;
				HasPlayed = false;
				CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1AoE1;

				if (BBC != nullptr)
				{
					BBC->SetValueAsInt("IsAttacking", false);
				}
			}
		}
	}
}

void ABoss::Melee2bLeft()
{
	if (!GetMesh()->IsPlaying() && !HasPlayed)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Left Slap Begin"));
		ShouldDamage = true;
		CalculateHandLocation();
		GetMesh()->PlayAnimation(MeleeAttack2bLeftTo, false);
		HasPlayed = true;

		if (BBC != nullptr)
		{
			BBC->SetValueAsInt("IsAttacking", true);
		}
	}

	if (!GetMesh()->IsPlaying() && HasPlayed || FirstAnimFinished)
	{
		FirstAnimFinished = true;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Left Slap End"));
		ShockWave();
		ShouldDamage = false;

		if (MeleeAttack2bLeftTimeDown <= MeleeAttack2bLeftTimer)
		{
			if (!GetMesh()->IsPlaying() && FirstAnimFinished && !SecondAnimStarted)
			{
				GetMesh()->PlayAnimation(MeleeAttack2bLeftReturn, false);
				SecondAnimStarted = true;
			}

			if (!GetMesh()->IsPlaying() && SecondAnimStarted)
			{
				FirstAnimFinished = false;
				SecondAnimStarted = false;
				MeleeAttack2bLeftTimer = 0;
				HasPlayed = false;
				CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1AoE1;

				if (BBC != nullptr)
				{
					BBC->SetValueAsInt("IsAttacking", false);
				}
			}
		}
	}

}

void ABoss::AoE1()
{
	if (AoE1SpawnCounter >= AoE1AmountToSpawn.Num())
	{
		if (AoE1DelayBetweenSpawns <= AoE1TimerBetweenSpawns)
		{
			AoE1TimerBetweenSpawns = 0;
			if (HandsAlive() == 2)
			{
				PhaseSection = PhaseSection + 1 > 2 ? 1 : PhaseSection + 1;

				switch (PhaseSection)
				{
				case 1:
				{
					CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee1L;
					break;
				}
				case 2:
				{
					CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee1R;
				}
				default:
					break;
				}
			}
			else if(HandsAlive() == 1)
			{
				if (LeftHandAlive)
				{
					CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee1L;
				}
				else if(RightHandAlive)
				{
					CurrentAttack = CurrentAttack == BossAttacks::Waiting ? BossAttacks::Waiting : BossAttacks::P1Melee1R;
				}
			}
			else
			{
				//End;
			}

			if (BBC != nullptr)
			{
				BBC->SetValueAsInt("IsAttacking", false);
			}
		}
	}

	if (AoE1DelayBetweenSpawns <= AoE1TimerBetweenSpawns)
	{
		if (BBC != nullptr)
		{
			BBC->SetValueAsInt("IsAttacking", true);
		}

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

int ABoss::HandsAlive()
{
	int LivingHands = 2;

	if (LeftHandCrystal == nullptr)
	{
		LivingHands -= 1;
		LeftHandAlive = false;
	}
	else
	{
		if (LeftHandCrystal->GetIsDead())
		{
			LivingHands -= 1;
			LeftHandAlive = false;
		}
	}

	if (RightHandCrystal == nullptr)
	{
		LivingHands -= 1;
		RightHandAlive = false;
	}
	else
	{
		if (RightHandCrystal->GetIsDead())
		{
			LivingHands -= 1;
			RightHandAlive = false;
		}
	}
	return LivingHands;
}

void ABoss::SummonType1()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	FVector RandLocation;

	if (BBC != nullptr)
	{
		BBC->SetValueAsInt("IsSummoning", true);
	}

	for (int i = 0; i < AmountToSummonV1; i++)
	{
		RandLocation = CalculateSpawnLocation();
		RandLocation.Z += ZSummonOffSet;

		//I'm about to tank the framerate lmao
		//Very sad
		//But if people don't know about it then I can't be asked to fix it
		SummonedEnemies.Add(GetWorld()->SpawnActor<AEnemy>(Summon1EnemyTypeBP, RandLocation, GetActorRotation(), spawnParams));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned"));
	}

	if (BBC != nullptr)
	{
		BBC->SetValueAsInt("IsSummoning", false);
	}

	SpawnSet = false;
	CurrentSummon = 1;

}

void ABoss::SummonType2()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	FVector RandLocation;

	if (BBC != nullptr)
	{
		BBC->SetValueAsInt("IsSummoning", true);
	}

	for (int i = 0; i < AmountToSummonV1; i++)
	{
		RandLocation = CalculateSpawnLocation();
		RandLocation.Z += ZSummonOffSet;

		//Ah shit here we go again
		//Plz send help
		SummonedEnemies.Add(GetWorld()->SpawnActor<AEnemy>(Summon2EnemyTypeBP, RandLocation, GetActorRotation(), spawnParams));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned"));
	}

	if (BBC != nullptr)
	{
		BBC->SetValueAsInt("IsSummoning", false);
	}

	SpawnSet = false;
	CurrentSummon = 2;
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
		if (Missile != nullptr)
		{
			Missile->Damage = MissileDamage;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Missile Spawn"));
	}
	else
	{
		////GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Has Fired"));
		//if (Missile == nullptr)
		//{
		//	HasFired = false;
		//	CurrentAttack = BossAttacks::Waiting;

		//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Destroy Pointer"));
		//}
		//else
		//{
		//	if (Missile->GetHasDestruct())
		//	{
		//		HasFired = false;
		//		CurrentAttack = BossAttacks::Waiting;

		//		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Destroy Fuction"));
		//	}
		//}


		if (TimeToNext <= TimerToNext)
		{
			HasFired = false;
			TimerToNext = 0;
			CurrentAttack = BossAttacks::Waiting;
		}
	}
}

void ABoss::ShouldEndPhase1()
{
	if (HandsAlive() == 0)
	{
		CurrentAttack == BossAttacks::Waiting;

		ChangePhase();
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
		if (Projectile != nullptr)
		{
			Projectile->Damage = RegProjectileDamage;
		}
		
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
	CurrentAttack = BossAttacks::Waiting;
	Phase += 1;
	if (BBC != nullptr)
	{
		BBC->SetValueAsInt("Phase", Phase);
	}
}

void ABoss::Phase2AttackChoice()
{
	if (isActiveMissile && isActiveRegProjectile)
	{
		switch (FMath::RandRange(0, 1))
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
	else if(isActiveMissile)
	{
		CurrentAttack = BossAttacks::P2MissileProjectile;
	}
	else
	{
		CurrentAttack = BossAttacks::P2RegularProjectile;
	}
}

void ABoss::HarponSpawn()
{
	if (SummonedEnemies.Num() != 0)
	{
		for (AEnemy* enemy : SummonedEnemies)
		{
			if (enemy != nullptr)
			{
				if (enemy->GetIsDead())
				{
					SummonedEnemies.Remove(enemy);
				}
			}
			else
			{
				SummonedEnemies.Remove(enemy);
			}
		}

		if (SummonedEnemies.Num() == 0 && SpawnSet)
		{
			return;
		}
	}
	else if (SummonedEnemies.Num() == 0 && SpawnSet)
	{
		return;
	}

	if (SummonedEnemies.Num() == 1 && !SpawnSet)
	{
		if (CurrentSummon == 1)
		{
			SpawnSet = true;
			SummonedEnemies[0]->SetSpawnOnDeath(HarponPiece1);
		}
		else
		{
			SpawnSet = true;
			SummonedEnemies[0]->SetSpawnOnDeath(HarponPiece2);
		}
	}
	else if (SummonedEnemies.Num() <= 0 && !SpawnSet)
	{
		AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();

		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();

		if (CurrentSummon == 1)
		{
			if (HarponPiece1 != nullptr)
			{
				GetWorld()->SpawnActor<AActor>(HarponPiece1, Player->GetActorLocation(), GetActorRotation(), spawnParams);
			}
		}
		else
		{
			if (HarponPiece2 != nullptr)
			{
				if (CurrentSummon == 2)
				{
					GetWorld()->SpawnActor<AActor>(HarponPiece2, Player->GetActorLocation(), GetActorRotation(), spawnParams);
				}
			}
		}
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

	if (OtherActor->ActorHasTag("Ground"))
	{
		HasHandHitGround = true;
		ShouldDamage = false;

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

		switch (CurrentAttack)
		{
		case P1Melee1L:
		{
			Cast<ADamageable>(OtherActor)->DecreaseHealth(P1Melee1LDamage);
			break;
		}
		case P1Melee1R:
		{
			Cast<ADamageable>(OtherActor)->DecreaseHealth(P1Melee1RDamage);
			break;
		}
		case P1Melee2aL:
		{
			Cast<ADamageable>(OtherActor)->DecreaseHealth(P1Melee2aLDamage);
			break;
		}
		case P1Melee2aR:
		{
			Cast<ADamageable>(OtherActor)->DecreaseHealth(P1Melee2aRDamage);
			break;
		}
		case P1Melee2bL:
		{
			Cast<ADamageable>(OtherActor)->DecreaseHealth(P1Melee2bLDamage);
			break;
		}
		case P1Melee2bR:
		{
			Cast<ADamageable>(OtherActor)->DecreaseHealth(P1Melee2bRDamage);
			break;
		}
		default:
			break;
		}
	}
}
	
