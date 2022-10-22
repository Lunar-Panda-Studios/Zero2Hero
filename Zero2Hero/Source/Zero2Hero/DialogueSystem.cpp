// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystem.h"

// Sets default values
ADialogueSystem::ADialogueSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADialogueSystem::BeginPlay()
{
	Super::BeginPlay();

	Manager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

	HUDOverlay = Manager->LoadDialogueBox(HUDOverlayAsset);

	StartDialogue(0);

	
}

// Called every frame
void ADialogueSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isPlaying)
	{
		Timer += DeltaTime;

		if (CharMax == CharNum)
		{
			//Will likely be change to say til click
			if (Timer >= SayOnScreenTimer)
			{
				isPlaying = false;
				Timer = 0;
			}
		}
		else
		{
			if (Timer >= TimeBetweenLetters)
			{
				CurrentDisplay = CurrentDisplay + FullDisplay[CharNum];

				HUDOverlay->TextToDisplay = CurrentDisplay;

				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, CurrentDisplay);

				CharNum += 1;
				Timer = 0;
			}
		}
	}
}

void ADialogueSystem::StartDialogue(int DialogueID)
{
	FullDisplay = DialogueInScene[DialogueID].Text;
	Audio = DialogueInScene[DialogueID].Audio;
	//DisplayBreakdown = FullDisplay.GetCharArray();
	CharMax = FullDisplay.Len();
	CurrentDisplay = "";
	UGameplayStatics::PlaySoundAtLocation(this, Audio, GetActorLocation());

	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	isPlaying = true;
}

FString ADialogueSystem::GetCurrentDisplay()
{
	return CurrentDisplay;
}

