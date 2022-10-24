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