// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveSystem.generated.h"

USTRUCT(BlueprintType)
struct FPowerCoreLocation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Core")
		FName PowerCoreColour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Core")
		bool InInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Core")
		bool BeenPlaced;
};

UCLASS()
class ZERO2HERO_API USaveSystem : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY()
		FVector CheckPointSpawn;
	UPROPERTY()
		TArray<struct FPowerCoreLocation> PowerCores;
	UPROPERTY()
		bool IsBridgeCompleted;
	UPROPERTY()
		TMap<FName, float> Ammos;

public:
	UFUNCTION()
		void SetCheckPointSpawn(FVector CPLocation);
	UFUNCTION()
		void SetPowerCoreLocation(TArray<FPowerCoreLocation> PowerCore);
	UFUNCTION()
		void SetBridgeComplete(bool bridgeStatus);
	UFUNCTION()
		void SetRangedWeapons(TMap<FName, float> Ammo);

	UFUNCTION()
		FVector GetCPSpawn();
	UFUNCTION()
		TArray<FPowerCoreLocation> GetPowerCoreLocations();
	UFUNCTION()
		bool GetBridgeStatus();
	UFUNCTION()
		TMap<FName, float> GetAmmos();
};
