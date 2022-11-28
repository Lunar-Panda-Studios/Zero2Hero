#include "EventZone.h"
#include "Kismet\KismetSystemLibrary.h"

AEventZone::AEventZone()
{
}

AEventZone::~AEventZone()
{
}

void AEventZone::BeginPlay()
{
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

void AEventZone::Tick(float DeltaTime)
{
	CheckEnemies();
	
}

void AEventZone::CheckEnemies()
{
	for (int i = 0; i < enemies.Num(); i++)
	{
		if (enemies[i] && enemies[i]->GetHealth() > 0)
		{
			if (!enemies[i]->IsValidLowLevel())
			{
				enemies.RemoveAt(i, 1, true);
			}
			if (enemies[i]->IsPendingKillPending())
			{
				enemies.RemoveAt(i, 1, true);
			}
		}
		else
		{
			enemies.RemoveAt(i, 1, true);
		}
	}
	
}

void AEventZone::RemoveEnemy(AEnemy* enemy)
{
	enemies.Remove(enemy);
}


