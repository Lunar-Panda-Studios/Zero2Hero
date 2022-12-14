// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DialogueBox.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem.h"
#include "OptionsSave.h"
#include "GameManager.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API UGameManager : public UGameInstance
{
	GENERATED_BODY()
protected:

	virtual void OnStart();

	UPROPERTY(BlueprintReadWrite)
		bool InBossFight = false;

	UPROPERTY(BlueprintReadWrite)
		FVector CurrentCheckPoint;
	UPROPERTY()
		int CheckPointNum = -1;
	UPROPERTY(BlueprintReadWrite)
		TArray<FPowerCoreLocation> PowerCores;

	UPROPERTY(BlueprintReadWrite)
		FPowerCoreLocation PowerCoresRed;
	UPROPERTY(BlueprintReadWrite)
		FPowerCoreLocation PowerCoresGreen;
	UPROPERTY(BlueprintReadWrite)
		FPowerCoreLocation PowerCoresYellow;
	UPROPERTY(BlueprintReadWrite)
		FPowerCoreLocation PowerCoresBlue;

	UPROPERTY()
		bool BridgeStatus;
	UPROPERTY()
		TMap<FName, float> Ammo;
	UPROPERTY()
		float IceAmmo;
	UPROPERTY()
		float ElectricAmmo;
	UPROPERTY()
		float NatureAmmo;
	UPROPERTY()
		float FireAmmo;
	UPROPERTY(BlueprintReadWrite)
		bool TowerOpen = false;

	UPROPERTY(BlueprintReadWrite)
		bool LoadingSave = false;
	UPROPERTY()
		bool LoadingIn = false;

	//For Options Save 
	UPROPERTY(BlueprintReadWrite)
		float AppliedMasterVol = 0.5f;
	UPROPERTY(BlueprintReadWrite)
		float AppliedMusicVol = 0.5f;
	UPROPERTY(BlueprintReadWrite)
		float AppliedSFXVol = 0.5f;
	UPROPERTY(BlueprintReadWrite)
		float AppliedVoiceVol = 0.5f;
	UPROPERTY(BlueprintReadWrite)
		bool AppliedSubtiles = true;
	UPROPERTY(BlueprintReadWrite)
		float AppliedMouseSense = 2.0f;

public:
	UFUNCTION()
		bool GetLoadingSave();
	UFUNCTION()
		bool GetLoadingIn();
	UFUNCTION()
		void SetLoadingIn(bool newLoadingIn);
	UFUNCTION()
		UDialogueBox* LoadDialogueBox(TSubclassOf<class UUserWidget> Asset);
	UFUNCTION()
		void SetCurrentCheckPoint(FVector newCPLocation);
	UFUNCTION()
		bool SetCurrentCheckPointNum(int newCPNum);
	UFUNCTION()
		bool GetIsInBossFight();

	UFUNCTION()
		FVector GetCurrentCP();
	UFUNCTION()
		TArray<FPowerCoreLocation> GetPowerCores();
	UFUNCTION()
		bool GetBridgeStatus();
	UFUNCTION()
		TMap<FName, float> GetAmmo();

	UFUNCTION()
		float GetIceAmmo();
	UFUNCTION()
		void SetIceAmmo(float newAmmo);
	UFUNCTION()
		float GetFireAmmo();
	UFUNCTION()
		void SetFireAmmo(float newAmmo);
	UFUNCTION()
		float GetNatureAmmo();
	UFUNCTION()
		void SetNatureAmmo(float newAmmo);
	UFUNCTION()
		float GetElectricAmmo();
	UFUNCTION()
		void SetElectricAmmo(float newAmmo);
	UFUNCTION()
		bool GetTowerOpen();
	UFUNCTION()
		void SetTowerOpen(bool newOpen);

	UFUNCTION(BlueprintCallable)
		void addPowerCore(FName Colour, bool InInventory, bool Placed);

	UFUNCTION(BlueprintCallable)
		void Respawn(AActor* Player);
	UFUNCTION(BlueprintCallable)
		void SaveGame(TSubclassOf<USaveSystem> Save);
	UFUNCTION(BlueprintCallable)
		bool LoadGame();
	UFUNCTION()
		FVector GetCurrentCheckPoint();
	UFUNCTION(BlueprintImplementableEvent)
		void ReloadBossScene();

	UFUNCTION(BlueprintCallable)
		void SaveOptions(TSubclassOf<USaveSystem> Save);
	UFUNCTION(BlueprintCallable)
		bool LoadOptions();
	
};
