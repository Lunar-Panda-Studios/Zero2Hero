// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RespawnPoint = CreateDefaultSubobject<USphereComponent>(TEXT("RespawnPoint - Center"));
	//RespawnPoint->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
	Manager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

	Mesh = FindComponentByClass<UStaticMeshComponent>();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::ActiveCheckPoint);
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckPoint::ActiveCheckPoint(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("Overlap Checkpoint"));
	if (OtherActor->ActorHasTag("Player"))
	{
		if (!OtherComp->ComponentHasTag("PlayerView"))
		{
			if (Manager != nullptr)
			{
				if (Manager->SetCurrentCheckPointNum(CheckPointNum))
				{
					Manager->SetCurrentCheckPoint(RespawnPoint->GetComponentLocation());
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("CheckPoint Got"));
				}
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("No GameManger"));
			}
		}
	}
}

