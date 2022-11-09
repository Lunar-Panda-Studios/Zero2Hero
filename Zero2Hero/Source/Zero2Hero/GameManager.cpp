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

void UGameManager::Respawn(AActor* Player)
{
	Cast<ACharacter>(Player)->LaunchCharacter(FVector(0, 0, 0),true,true);
	Player->SetActorLocation(CurrentCheckPoint);
}
