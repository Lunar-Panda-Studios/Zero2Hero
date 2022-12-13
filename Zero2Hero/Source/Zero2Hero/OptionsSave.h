// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveSystem.h"
#include "OptionsSave.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API UOptionsSave : public USaveSystem
{
	GENERATED_BODY()

	UPROPERTY()
		float MasterVol;
	UPROPERTY()
		float MusicVol;
	UPROPERTY()
		float SFXVol;
	UPROPERTY()
		float VoiceVol;
	UPROPERTY()
		bool Subtiles;
	UPROPERTY()
		float Sensitivity;

public:

	UFUNCTION()
		void SetMasterVol(float newVol);
	UFUNCTION()
		void SetMusicVol(float newVol);
	UFUNCTION()
		void SetSFXVol(float newVol);
	UFUNCTION()
		void SetVoiceVol(float newVol);
	UFUNCTION()
		void SetSubTitles(bool newSubtitles);
	UFUNCTION()
		void SetSensetivity(float newSensitivity);

	UFUNCTION()
		float GetMasterVol();
	UFUNCTION()
		float GetMusicVol();
	UFUNCTION()
		float GetSFXVol();
	UFUNCTION()
		float GetVoiceVol();
	UFUNCTION()
		bool GetSubtitles();
	UFUNCTION()
		float GetSensetivity();
	
};
