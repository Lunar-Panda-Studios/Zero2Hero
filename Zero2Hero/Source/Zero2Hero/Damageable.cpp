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
					FActorSpawnParameters spawnParams;
					spawnParams.Owner = this;
					spawnParams.Instigator = GetInstigator();

					GetWorld()->SpawnActor<AActor>(SpawnOnDeath, GetActorLocation(), GetActorRotation(), spawnParams);
				}
				Destroy();
			}
		}
	}

	CheckDeath();
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
}

void ADamageable::SetIsDead(bool newDead)
{
	isDead = newDead;
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
		Health -= amount;
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

void ADamageable::SetHealth(int amount)
{
	Health = amount;
}

void ADamageable::CheckDeath()
{
	if (Health <= 0 && !isDead)
	{
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
		PairedEnemy->SetEnemyPair(nullptr);
		PairedEnemy = nullptr;
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

