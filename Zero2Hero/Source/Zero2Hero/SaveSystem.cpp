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

void USaveSystem::SetRangedWeapons(float ice, float fire, float nature, float electric)
{
	IceAmmo = ice;
	FireAmmo = fire;
	NatureAmmo = nature;
	ElectricAmmo = electric;
}

void USaveSystem::SetTowerOpen(bool newOpen)
{
	TowerOpen = newOpen;
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

float USaveSystem::GetIceAmmo()
{
	return IceAmmo;
}

float USaveSystem::GetFireAmmo()
{
	return FireAmmo;
}

float USaveSystem::GetNatureAmmo()
{
	return NatureAmmo;
}

float USaveSystem::GetElectricAmmo()
{
	return ElectricAmmo;
}

bool USaveSystem::GetTowerOpen()
{
	return TowerOpen;
}
