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

bool UGameManager::GetIsInBossFight()
{
	return InBossFight;
}

bool UGameManager::GetTowerOpen()
{
	return TowerOpen;
}

void UGameManager::SetTowerOpen(bool newOpen)
{
	TowerOpen = newOpen;
}

void UGameManager::SaveOptions(TSubclassOf<USaveSystem> Save)
{
	UOptionsSave* Saved = Cast<UOptionsSave>(UGameplayStatics::CreateSaveGameObject(Save));

	Saved->SetMasterVol(AppliedMasterVol);
	Saved->SetSFXVol(AppliedSFXVol);
	Saved->SetMusicVol(AppliedMusicVol);
	Saved->SetVoiceVol(AppliedVoiceVol);
	Saved->SetSubTitles(AppliedSubtiles);
	Saved->SetSensetivity(AppliedMouseSense);

	UGameplayStatics::SaveGameToSlot(Saved, "EQUI&AcaOption", 0);
}

bool UGameManager::LoadOptions()
{
	UOptionsSave* Save = Cast<UOptionsSave>(UGameplayStatics::LoadGameFromSlot("EQUI&AcaOption", 0));

	if (Save == nullptr)
	{
		return false;
	}

	AppliedMasterVol = Save->GetMasterVol();
	AppliedSFXVol = Save->GetSFXVol();
	AppliedMusicVol = Save->GetMusicVol();
	AppliedVoiceVol = Save->GetVoiceVol();
	AppliedSubtiles = Save->GetSubtitles();
	AppliedMouseSense = Save->GetSensetivity();

	return true;
}

FVector UGameManager::GetCurrentCheckPoint()
{
	return CurrentCheckPoint;
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

bool UGameManager::GetLoadingIn()
{
	return LoadingIn;
}

void UGameManager::Respawn(AActor* Player)
{
	Cast<ACharacter>(Player)->LaunchCharacter(FVector(0, 0, 0),true,true);

	if (!InBossFight)
	{
		Player->SetActorLocation(CurrentCheckPoint);
	}
	else
	{
		ReloadBossScene();
	}
}

void UGameManager::SetLoadingIn(bool newLoadingIn)
{
	LoadingIn = newLoadingIn;
}

void UGameManager::SaveGame(TSubclassOf<USaveSystem> Save)
{
	if (!LoadingIn)
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
		Saved->SetTowerOpen(TowerOpen);

		//FString SaveFile = "EQUI&AcaData" + FDateTime::Now().ToString();

		UGameplayStatics::SaveGameToSlot(Saved, "EQUI&AcaData", 0);
	}
}

bool UGameManager::LoadGame()
{
	USaveSystem* Save = Cast<USaveSystem>(UGameplayStatics::LoadGameFromSlot("EQUI&AcaData", 0));

	if (Save == nullptr)
	{
		LoadingIn = false;
		LoadingSave = false;
		return false;
	}

	CurrentCheckPoint = Save->GetCPSpawn();
	BridgeStatus = Save->GetBridgeStatus();
	PowerCores = Save->GetPowerCoreLocations();
	IceAmmo = Save->GetIceAmmo();
	ElectricAmmo = Save->GetElectricAmmo();
	NatureAmmo = Save->GetNatureAmmo();
	FireAmmo = Save->GetFireAmmo();
	TowerOpen = Save->GetTowerOpen();

	LoadingIn = true;
	LoadingSave = true;
	return true;
}
