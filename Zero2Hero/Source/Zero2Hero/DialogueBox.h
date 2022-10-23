// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueBox.generated.h"

/**
 * 
 */
UCLASS()
class ZERO2HERO_API UDialogueBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TextToDisplay;
	
};
