// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

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

void UGameManager::addPowerCore(FName Colour, bool InInventory, bool Placed)
{
	FPowerCoreLocation PowerCore;
	PowerCore.PowerCoreColour = Colour;
	PowerCore.InInventory = InInventory;
	PowerCore.BeenPlaced = Placed;

	bool BeenAdded = false;

	for (FPowerCoreLocation Cores : PowerCores)
	{
		if (Cores.PowerCoreColour == PowerCore.PowerCoreColour)
		{
			Cores.InInventory = InInventory;
			Cores.BeenPlaced = Placed;

			BeenAdded = true;

			break;
		}
	}

	if (!BeenAdded)
	{
		PowerCores.Add(PowerCore);
	}
}

void UGameManager::Respawn(AActor* Player)
{
	Cast<ACharacter>(Player)->LaunchCharacter(FVector(0, 0, 0),true,true);
	Player->SetActorLocation(CurrentCheckPoint);
}

void UGameManager::SaveGame(TSubclassOf<USaveSystem> Save, TMap<FName, float> AmmoTypes)
{
	USaveSystem* Saved = Cast<USaveSystem>(UGameplayStatics::CreateSaveGameObject(Save));

	Saved->SetCheckPointSpawn(CurrentCheckPoint);
	Saved->SetBridgeComplete(BridgeStatus);
	Saved->SetRangedWeapons(Ammo);
	Saved->SetPowerCoreLocation(PowerCores);

	UGameplayStatics::SaveGameToSlot(Saved, "EQUI&AcaData", 0);
}

void UGameManager::LoadGame()
{
	USaveSystem* Save = Cast<USaveSystem>(UGameplayStatics::LoadGameFromSlot("EQUI&AcaData", 0));

	CurrentCheckPoint = Save->GetCPSpawn();
	BridgeStatus = Save->GetBridgeStatus();
	PowerCores = Save->GetPowerCoreLocations();
	Ammo = Save->GetAmmos();
}
