// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamingActor.h"

// Sets default values
ALevelStreamingActor::ALevelStreamingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;
}

// Called when the game starts or when spawned
void ALevelStreamingActor::BeginPlay()
{
	Super::BeginPlay();

	if (ShouldLoadLevel)
	{
		OverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelStreamingActor::OverlapBegins);
	}

	if (ShouldUnloadLevel)
	{
		OverlapVolume->OnComponentEndOverlap.AddDynamic(this, &ALevelStreamingActor::OverlapEnds);
	}
	
}

void ALevelStreamingActor::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlap Start"));
	if (OtherActor->ActorHasTag("Player") && !OtherComp->ComponentHasTag("Ignore") && LevelToLoad != "")
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("LoadStreamLevel"));
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
	}
}

void ALevelStreamingActor::OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlap End"));

	if (OtherActor->ActorHasTag("Player") && !OtherComp->ComponentHasTag("Ignore") && LevelToUnload != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToUnload, LatentInfo, true);
	}
}

// Called every frame
void ALevelStreamingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

