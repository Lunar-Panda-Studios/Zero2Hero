// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem.h"

void USaveSystem::SetCheckPointSpawn(FVector CPLocation)
{
	CheckPointSpawn = CPLocation;
}

void USaveSystem::SetPowerCoreLocation(TArray<FPowerCoreLocation> PowerCore)
{
	PowerCores = PowerCore;
}

void USaveSystem::SetBridgeComplete(bool bridgeStatus)
{
	IsBridgeCompleted = bridgeStatus;
}

void USaveSystem::SetRangedWeapons(TMap<FName,float> Ammo)
{
	Ammos = Ammo;
}

FVector USaveSystem::GetCPSpawn()
{
	return CheckPointSpawn;
}

TArray<FPowerCoreLocation> USaveSystem::GetPowerCoreLocations()
{
	return PowerCores;
}

bool USaveSystem::GetBridgeStatus()
{
	return IsBridgeCompleted;
}

TMap<FName, float> USaveSystem::GetAmmos()
{
	return Ammos;
}
