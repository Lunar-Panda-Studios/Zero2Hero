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
}

// Called every frame
void ADamageable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (isDead)
	//{
	//	AnimationTimer += DeltaTime;
	//	if (AnimationTime <= AnimationTimer)
	//	{
	//		Destroy(ac)
	//	}
	//}
}

int ADamageable::GetDamage()
{
	return Damage;
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
	Health -= amount;
}

void ADamageable::CheckDeath()
{
	if (Health <= 0)
	{
		isDead = true;
		SetLifeSpan(AnimationTimer);
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

