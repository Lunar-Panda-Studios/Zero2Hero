// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

void UGameManager::OnStart()
{
	FPowerCoreLocation PowerCore;
	PowerCore.BeenPlaced = false;
	PowerCore.InInventory = false;

	PowerCore.PowerCoreColour = "Blue";
	PowerCoresBlue = PowerCore;

	PowerCore.PowerCoreColour = "Green";
	PowerCoresGreen = PowerCore;

	PowerCore.PowerCoreColour = "Yellow";
	PowerCoresYellow = PowerCore;

	PowerCore.PowerCoreColour = "Red";
	PowerCoresRed = PowerCore;

	if (LoadingSave)
	{
		LoadGame();
	}
}

bool UGameManager::GetLoadingSave()
{
	return LoadingSave;
}

UDialogueBox* UGameManager::LoadDialogueBox(TSubclassOf<class UUserWidget> Asset)
{
	UDialogueBox* HUDOverlay;

	if(Asset)
	{
		HUDOverlay = CreateWidget<UDialogueBox>(this, Asset);
		HUDOverlay->AddToViewport();

		return HUDOverlay;
	}

	return nullptr;

}

void UGameManager::SetCurrentCheckPoint(FVector newCPLocation)
{
	CurrentCheckPoint = newCPLocation;
}

bool UGameManager::SetCurrentCheckPointNum(int newCPNum)
{
	if (newCPNum > CheckPointNum)
	{
		CheckPointNum = newCPNum;
		return true;
	}

	return false;
}

FVector UGameManager::GetCurrentCP()
{
	return CurrentCheckPoint;
}

TArray<FPowerCoreLocation> UGameManager::GetPowerCores()
{
	return PowerCores;	
}

bool UGameManager::GetBridgeStatus()
{
	return BridgeStatus;
}

TMap<FName, float> UGameManager::GetAmmo()
{
	return Ammo;
}

float UGameManager::GetIceAmmo()
{
	return IceAmmo;
}

float UGameManager::GetFireAmmo()
{
	return FireAmmo;
}

float UGameManager::GetNatureAmmo()
{
	return NatureAmmo;
}

float UGameManager::GetElectricAmmo()
{
	return ElectricAmmo;
}

void UGameManager::SetIceAmmo(float newAmmo)
{
	IceAmmo = newAmmo;
}

void UGameManager::SetFireAmmo(float newAmmo)
{
	FireAmmo = newAmmo;
}

void UGameManager::SetNatureAmmo(float newAmmo)
{
	NatureAmmo = newAmmo;
}

void UGameManager::SetElectricAmmo(float newAmmo)
{
	ElectricAmmo = newAmmo;
}

void UGameManager::addPowerCore(FName Colour, bool InInventory, bool Placed)
{
	if (Colour == PowerCoresBlue.PowerCoreColour)
	{
		PowerCoresBlue.InInventory = InInventory;
		PowerCoresBlue.BeenPlaced = Placed;
	}
	else if (Colour == PowerCoresYellow.PowerCoreColour)
	{
		PowerCoresYellow.InInventory = InInventory;
		PowerCoresYellow.BeenPlaced = Placed;
	}
	else if (Colour == PowerCoresRed.PowerCoreColour)
	{
		PowerCoresRed.InInventory = InInventory;
		PowerCoresRed.BeenPlaced = Placed;
	}
	else if (Colour == PowerCoresGreen.PowerCoreColour)
	{
		PowerCoresGreen.InInventory = InInventory;
		PowerCoresGreen.BeenPlaced = Placed;
	}
}

void UGameManager::Respawn(AActor* Player)
{
	Cast<ACharacter>(Player)->LaunchCharacter(FVector(0, 0, 0),true,true);
	Player->SetActorLocation(CurrentCheckPoint);
}

void UGameManager::SaveGame(TSubclassOf<USaveSystem> Save)
{
	PowerCores.Empty();

	PowerCores.Add(PowerCoresYellow);
	PowerCores.Add(PowerCoresGreen);
	PowerCores.Add(PowerCoresRed);
	PowerCores.Add(PowerCoresBlue);

	USaveSystem* Saved = Cast<USaveSystem>(UGameplayStatics::CreateSaveGameObject(Save));

	Saved->SetCheckPointSpawn(CurrentCheckPoint);
	Saved->SetBridgeComplete(BridgeStatus);
	Saved->SetRangedWeapons(IceAmmo, FireAmmo, NatureAmmo, ElectricAmmo);
	Saved->SetPowerCoreLocation(PowerCores);

	//FString SaveFile = "EQUI&AcaData" + FDateTime::Now().ToString();

	UGameplayStatics::SaveGameToSlot(Saved, "EQUI&AcaData", 0);
}

void UGameManager::LoadGame()
{
	USaveSystem* Save = Cast<USaveSystem>(UGameplayStatics::LoadGameFromSlot("EQUI&AcaData", 0));

	CurrentCheckPoint = Save->GetCPSpawn();
	BridgeStatus = Save->GetBridgeStatus();
	PowerCores = Save->GetPowerCoreLocations();
	IceAmmo = Save->GetIceAmmo();
	ElectricAmmo = Save->GetElectricAmmo();
	NatureAmmo = Save->GetNatureAmmo();
	FireAmmo = Save->GetFireAmmo();
}
