// Fill out your copyright notice in the Description page of Project Settings.


#include "Damageable.h"

// Sets default values
ADamageable::ADamageable()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADamageable::BeginPlay()
{
	Super::BeginPlay();
	
	//Sets health
	Health = MaxHealth;

	Manager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
}

// Called every frame
void ADamageable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isDead && !beingRevived)
	{
		if (isShielded)
		{
			UnshieldEnemy();
		}

		AnimationTimer += DeltaTime;
		if (AnimationTime <= AnimationTimer)
		{
			if (ActorHasTag("Player"))
			{
				AnimationTimer = 0;
				Manager->Respawn(this);
				Health = MaxHealth;
				isDead = false;
				Allow = true;
			}
			else
			{
				if (SpawnOnDeath != nullptr)
				{
					if (!HasSpawned && CanSpawn)
					{
						FHitResult Hit;

						FVector GroundLocation = GetActorLocation();

						AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();

						FVector LineTraceEnd = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 2000);
						FCollisionQueryParams TraceParams;
						TraceParams.AddIgnoredActor(Player);
						TraceParams.AddIgnoredActor(this);

						GetWorld()->LineTraceSingleByChannel(OUT Hit, GetActorLocation(), LineTraceEnd, ECollisionChannel::ECC_Camera, TraceParams, FCollisionResponseParams());

						if (Hit.IsValidBlockingHit())
						{
							GroundLocation.Z = Hit.ImpactPoint.Z;
						}

						FActorSpawnParameters spawnParams;
						spawnParams.Owner = this;
						spawnParams.Instigator = GetInstigator();

						GetWorld()->SpawnActor<AActor>(SpawnOnDeath, GroundLocation, GetActorRotation(), spawnParams);

						HasSpawned = true;
					}
				}

				if (!IsCrystal)
				{
					Destroy();
				}
			}
		}
	}

	CheckDeath();
}

void ADamageable::SetCanSpawn(bool newSpawn)
{
	CanSpawn = newSpawn;
}

int ADamageable::GetDamage()
{
	return Damage;
}

bool ADamageable::GetIsDead()
{
	return isDead;
}

bool ADamageable::GetBeingRevived()
{
	return beingRevived;
}

void ADamageable::SetBeingRevived(bool newRevive)
{
	beingRevived = newRevive;

	if(!beingRevived)
	{
		AnimationTimer = 0;
		EnemyDies();
	}
}

void ADamageable::SetIsDead(bool newDead)
{
	isDead = newDead;
}

void ADamageable::SetIsCrystal(bool NewIsCrystal)
{
	IsCrystal = NewIsCrystal;
}

void ADamageable::OnDeath()
{

}


float ADamageable::GetHealth()
{
	return Health;
}

float ADamageable::GetMaxHealth()
{
	return MaxHealth;
}

void ADamageable::IncreaseHealth(int amount)
{
	Health += amount;
}

void ADamageable::DecreaseHealth(int amount)
{
	if (!isDead)
	{
		if (CanDamage)
		{
			if (!isBoss)
			{
				Health -= amount;
			}
			if (ActorHasTag("Enemy"))
			{
				EnemyDamaged();
			}
			else if (ActorHasTag("Player"))
			{
				PlayerDamaged();
			}
			CheckDeath();
		}
	}
}

void ADamageable::SetHealth(int amount)
{
	Health = amount;
}

void ADamageable::CheckDeath()
{
	if (Health <= 0 && !isDead)
	{
		OnDeath();
		if (ActorHasTag("Enemy"))
		{
			EnemyDies();
		}
		else if (ActorHasTag("Player"))
		{
			PlayerDies();
		}
		isDead = true;
		//SetLifeSpan(AnimationTimer);
		AnimationTimer = 0;
		Allow = false;
	}
}

	bool ADamageable::GetIsShielded()
	{
		return isShielded;
	}

	bool ADamageable::GetIsShieldReflect()
	{
		return ReflectorShield;
	}

	TEnumAsByte<ElementType> ADamageable::GetShieldType()
	{
		return CurrentShieldType;
	}

	void ADamageable::UnshieldEnemy()
	{
		isShielded = false;
		if (PairedEnemy != nullptr)
		{
			PairedEnemy->SetEnemyPair(nullptr);
			PairedEnemy = nullptr;
		}
		CurrentShieldType = ElementType::None;
	}

	void ADamageable::SetEnemyPair(ADamageable* newPair)
	{
		PairedEnemy = newPair;
	}

	void ADamageable::SetShielded(bool shield)
	{
		isShielded = shield;
	}

	void ADamageable::UnPair()
	{
		PairedEnemy = nullptr;
	}

	void ADamageable::SetShieldType(TEnumAsByte<ElementType> newElement)
	{
		CurrentShieldType = newElement;
		isShielded = true;
	}

	void ADamageable::SetisReflectorShield(bool isReflector)
	{
		ReflectorShield = isReflector;
	}

	void ADamageable::SetSpawnOnDeath(TSubclassOf<AActor> newDropItem)
	{
		SpawnOnDeath = newDropItem;
	}

