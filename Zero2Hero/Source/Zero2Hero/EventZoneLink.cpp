// Fill out your copyright notice in the Description page of Project Settings.


#include "EventZoneLink.h"

// Sets default values
AEventZoneLink::AEventZoneLink()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEventZoneLink::BeginPlay()
{
	Super::BeginPlay();
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ignoreActors.Init(this, 1);
	seekClass = UStaticMesh::StaticClass();
	DrawDebugSphere(GetWorld(), GetActorLocation(), radius, 12, FColor::Purple, true, 1000.0f, ESceneDepthPriorityGroup::SDPG_Foreground, 5.0f);
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), radius, traceObjectTypes, AActor::StaticClass(), ignoreActors, actors))
	{
		for (AActor* a : actors)
		{
			if (Cast<AEnemy>(a))
			{
				enemies.Add(Cast<AEnemy>(a));
			}
		}
	}
}

// Called every frame
void AEventZoneLink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (int i = 0; i < enemies.Num(); i++)
	{
		if (!enemies[i] || !(enemies[i]->GetHealth() > 0.0f))
		{
			enemies.RemoveAt(i, 1, true);
		}
	}

	if (enemies.Num() < 1)
	{
		AllEnemiesKilled();
		Destroy();
	}
}

