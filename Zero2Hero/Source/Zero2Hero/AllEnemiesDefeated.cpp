// Fill out your copyright notice in the Description page of Project Settings.


#include "AllEnemiesDefeated.h"

// Sets default values
AAllEnemiesDefeated::AAllEnemiesDefeated()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAllEnemiesDefeated::BeginPlay()
{
	Super::BeginPlay();
	Manager = Cast<UGameManager>(GetWorld()->GetGameInstance());
}

// Called every frame
void AAllEnemiesDefeated::Tick(float DeltaTime)
{
	if (InUse)
	{
		CheckEnemyStatus();
	}
}

void AAllEnemiesDefeated::SetSpawnSet(bool NewSpawnSet)
{
	SpawnSet = NewSpawnSet;
}

void AAllEnemiesDefeated::SetInUse(bool newInUse)
{
	InUse = newInUse;
}

void AAllEnemiesDefeated::AddEnemy(AEnemy* Enemy)
{
	EnemiesToCheck.Add(Enemy);
}

void AAllEnemiesDefeated::CheckEnemyStatus()
{
	if (EnemiesToCheck.Num() == 0 && SpawnSet)
	{
		InUse = false;
	}

	for (int i = 0; i < EnemiesToCheck.Num(); i++)
	{
		if (!EnemiesToCheck[i] || !(EnemiesToCheck[i]->GetHealth() > 0.0f))
		{
			EnemiesToCheck.RemoveAt(i, 1, true);
		}
	}

	if (EnemiesToCheck.Num() == 1 )
	{
		EnemiesToCheck[0]->SetCanSpawn(true);
		SpawnSet = true;
	}
	else if(EnemiesToCheck.Num() == 0 && !SpawnSet)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();

		if (CurrentSummon == 1)
		{
			if (Harpoon1 != nullptr)
			{
				GetWorld()->SpawnActor<AActor>(Harpoon1, Manager->GetCurrentCheckPoint(), GetActorRotation(), spawnParams);
			}
		}
		else
		{
			if (Harpoon2 != nullptr)
			{
				if (CurrentSummon == 2)
				{
					GetWorld()->SpawnActor<AActor>(Harpoon2, Manager->GetCurrentCheckPoint(), GetActorRotation(), spawnParams);
				}
			}
		}
		SpawnSet = true;
		InUse = false;
	}
}

void AAllEnemiesDefeated::SetCurrentSummon(int newSummonV)
{
	CurrentSummon = newSummonV;
}

void AAllEnemiesDefeated::SetHarpoon1(TSubclassOf<AActor> Harpoon)
{
	Harpoon1 = Harpoon;
}

void AAllEnemiesDefeated::SetHarpoon2(TSubclassOf<AActor> Harpoon)
{
	Harpoon2 = Harpoon;
}

