// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadDistributor.h"

// Sets default values
ALoadDistributor::ALoadDistributor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALoadDistributor::BeginPlay()
{
	Super::BeginPlay();

	Manager = Cast<UGameManager>(GetWorld()->GetGameInstance());
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ALoadDistributor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ALoadDistributor::DistributeLoad()
{
	DistributeAmmo();
	DistributePlayerLocation();
	return true;
}

FPowerCoreLocation ALoadDistributor::DistributePowerCore(FName Colour)
{
	if (Manager == nullptr)
	{
		Manager = Cast<UGameManager>(GetWorld()->GetGameInstance());
	}

		TArray<FPowerCoreLocation> PowerCores = Manager->GetPowerCores();
		FPowerCoreLocation WantedPowerCore;

		for (int i = 0; i < PowerCores.Num(); i++)
		{
			if (PowerCores[i].PowerCoreColour == Colour)
			{
				WantedPowerCore = PowerCores[i];
				break;
			}
		}

	return WantedPowerCore;
}

bool ALoadDistributor::DistributeBridgeStatus()
{
	return Manager->GetBridgeStatus();
}

bool ALoadDistributor::DistributePlayerLocation()
{
	if (Player == nullptr || Manager == nullptr)
	{
		Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		Manager = Cast<UGameManager>(GetWorld()->GetGameInstance());
	}

	if (Player != nullptr && Manager != nullptr)
	{
		Player->SetActorLocation(Manager->GetCurrentCP());
	}
	return true;
}

bool ALoadDistributor::DistributeAmmo()
{
	if (Player == nullptr)
	{
		Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	for (int i = 0; i < Player->GetRangedWeapons().Num(); i++)
	{
		if (Player->GetRangedWeapons()[i]->GetWeaponName() == "Ice")
		{
			Player->GetRangedWeapons()[i]->SetAmmo(Manager->GetIceAmmo());
		}
		else if (Player->GetRangedWeapons()[i]->GetWeaponName() == "Fire")
		{
			Player->GetRangedWeapons()[i]->SetAmmo(Manager->GetFireAmmo());
		}
		else if (Player->GetRangedWeapons()[i]->GetWeaponName() == "Electric")
		{
			Player->GetRangedWeapons()[i]->SetAmmo(Manager->GetElectricAmmo());
		}
		else if (Player->GetRangedWeapons()[i]->GetWeaponName() == "Nature")
		{
			Player->GetRangedWeapons()[i]->SetAmmo(Manager->GetNatureAmmo());
		}

		return false;
	}

	return true;
}

