// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DialogueBox.h"
#include "GameFramework/Character.h"
#include "GameManager.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API UGameManager : public UGameInstance
{
	GENERATED_BODY()
protected:	
	UPROPERTY(BlueprintReadWrite)
		FVector CurrentCheckPoint;
	UPROPERTY()
		int CheckPointNum = -1;

public:
	UFUNCTION()
		UDialogueBox* LoadDialogueBox(TSubclassOf<class UUserWidget> Asset);
	UFUNCTION()
		void SetCurrentCheckPoint(FVector newCPLocation);
	UFUNCTION()
		bool SetCurrentCheckPointNum(int newCPNum);
	UFUNCTION()
		FVector GetCurrentCP();
	UFUNCTION(BlueprintCallable)
		void Respawn(AActor* Player);
	
};
