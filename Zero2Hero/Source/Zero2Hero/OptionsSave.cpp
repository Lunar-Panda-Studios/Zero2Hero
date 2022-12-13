// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsSave.h"

void UOptionsSave::SetMasterVol(float newVol)
{
	MasterVol = newVol;
}

void UOptionsSave::SetMusicVol(float newVol)
{
	MusicVol = newVol;
}

void UOptionsSave::SetSFXVol(float newVol)
{
	SFXVol = newVol;
}

void UOptionsSave::SetVoiceVol(float newVol)
{
	VoiceVol = newVol;
}

void UOptionsSave::SetSubTitles(bool newSubtitles)
{
	Subtiles = newSubtitles;
}

void UOptionsSave::SetSensetivity(float newSensitivity)
{
	Sensitivity = newSensitivity;
}

float UOptionsSave::GetMasterVol()
{
	return MasterVol;
}

float UOptionsSave::GetMusicVol()
{
	return MusicVol;
}

float UOptionsSave::GetSFXVol()
{
	return SFXVol;
}

float UOptionsSave::GetVoiceVol()
{
	return VoiceVol;
}

bool UOptionsSave::GetSubtitles()
{
	return Subtiles;
}

float UOptionsSave::GetSensetivity()
{
	return Sensitivity;
}
