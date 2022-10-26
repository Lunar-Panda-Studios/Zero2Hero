// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Char.h"
#include "DialogueBox.h"
#include "GameManager.h"
#include "Components/AudioComponent.h"
#include "DialogueSystem.generated.h"

USTRUCT(BlueprintType)
struct FDialogue
{
	GENERATED_BODY()

	UPROPERTY()
		int DialogueID;
	UPROPERTY(EditAnywhere)
		FString Speaker;
	UPROPERTY(EditAnywhere)
		USoundBase* Audio;
	UPROPERTY(EditAnywhere)
		FString Text;
	UPROPERTY(EditAnywhere)
		int NextDialogueID = -1;
	UPROPERTY(EditAnywhere)
		bool DisableEverything = false;

};


UCLASS()
class ZERO2HERO_API ADialogueSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> HUDOverlayAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UDialogueBox* HUDOverlay;

	UPROPERTY()
		UGameManager* Manager;

	UPROPERTY()
		USoundBase* Audio;
	UPROPERTY()
		FString CurrentDisplay;
	UPROPERTY()
		FString FullDisplay;
	UPROPERTY()
		FDialogue CurrentDialogue;
	UPROPERTY(EditAnywhere)
		UAudioComponent* InUseAudio;

	UPROPERTY()
		float Timer = 0.0f;
	UPROPERTY(EditAnywhere)
		float TimeBetweenLetters = 0.1f;

	UPROPERTY(BlueprintReadWrite)
		bool isPlaying = false;
	UPROPERTY()
		int CharNum = 0;
	UPROPERTY()
		int CharMax = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FDialogue> DialogueInScene;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void StartDialogue(int DialogueID);
	UFUNCTION()
		FString GetCurrentDisplay();
	UFUNCTION()
		bool CheckNextDialogue();
	UFUNCTION(BlueprintCallable)
		void OnClick();
	UFUNCTION()
		UDialogueBox* GetDialogueWidget();
	UFUNCTION()
		FDialogue GetCurrentDialogue();

};
