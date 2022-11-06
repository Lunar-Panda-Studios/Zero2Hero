// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DialogueBox.h"
#include "GameManager.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION()
		UDialogueBox* LoadDialogueBox(TSubclassOf<class UUserWidget> Asset);
	
};
