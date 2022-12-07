// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCrystalWeakness.h"

void ABossCrystalWeakness::BeginPlay()
{
	Super::BeginPlay();

	MeshBody = FindComponentByClass<UStaticMeshComponent>();
	MeshBody->OnComponentBeginOverlap.AddDynamic(this, &ABossCrystalWeakness::OnCrystalOverlap);

	IsCrystal = true;
}

void ABossCrystalWeakness::OnCrystalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
	{
		return;
	}
	if (!OtherActor->ActorHasTag("Player"))
	{
		return;
	}
	if (OtherComp == nullptr)
	{
		return;
	}
	if (OtherComp->ComponentHasTag("MeleeZone"))
	{
		DecreaseHealth(Cast<ADamageable>(OtherActor)->GetDamage());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Take Damage"));
	}
}
