// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystem.h"

// Sets default values
ADialogueSystem::ADialogueSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InUseAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
}

// Called when the game starts or when spawned
void ADialogueSystem::BeginPlay()
{
	Super::BeginPlay();

	Manager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

	HUDOverlay = Manager->LoadDialogueBox(HUDOverlayAsset);

	//StartDialogue(0);

	HUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	
}

// Called every frame
void ADialogueSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("Tick"));

	if (isPlaying)
	{
		Timer += DeltaTime;

		/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("IsPlaying"));*/

		if (CharMax == CharNum)
		{
			isPlaying = false;
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
	if (!InUseAudio->IsPlaying())
	{
		CurrentDialogue = DialogueInScene[DialogueID];
		FullDisplay = CurrentDialogue.Text;
		Audio = CurrentDialogue.Audio;
		CharMax = FullDisplay.Len();
		CurrentDisplay = "";
		CharNum = 0;
		HUDOverlay->TextToDisplay = CurrentDisplay;
		//UGameplayStatics::PlaySoundAtLocation(this, Audio, GetActorLocation());

		isPlaying = true;

		InUseAudio->SetSound(Audio);
		InUseAudio->Play();

		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

FString ADialogueSystem::GetCurrentDisplay()
{
	return CurrentDisplay;
}

bool ADialogueSystem::CheckNextDialogue()
{
	if (CurrentDialogue.NextDialogueID >= 0)
	{
		StartDialogue(CurrentDialogue.NextDialogueID);
		return true;
	}
	else
	{
		return false;
	}
}

void ADialogueSystem::OnClick()
{
	if (HUDOverlay != nullptr)
	{
		if (HUDOverlay->GetIsVisible())
		{
			if (isPlaying)
			{
				CurrentDisplay = FullDisplay;
				HUDOverlay->TextToDisplay = CurrentDisplay;
				CharNum = CharMax;
				isPlaying = false;
				Timer = 0;
			}
			else if (!CheckNextDialogue() && !InUseAudio->IsPlaying())
			{
				HUDOverlay->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

UDialogueBox* ADialogueSystem::GetDialogueWidget()
{
	return HUDOverlay;
}

FDialogue ADialogueSystem::GetCurrentDialogue()
{
	return CurrentDialogue;
}

