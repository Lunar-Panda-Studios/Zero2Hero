// Fill out your copyright notice in the Description page of Project Settings.
#include "TurretSeed.h"
#include "Turret.h"
#include "Kismet\KismetSystemLibrary.h"


ATurretSeed::ATurretSeed()
{

}

void ATurretSeed::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(seedLifeSpan);
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ignoreActors.Init(this, 1);
	seekClass = UStaticMesh::StaticClass();
}

void ATurretSeed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	actors.Empty();
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), seedRadius, traceObjectTypes, AActor::StaticClass(), ignoreActors, actors);
	bool spawned = false;
	for (AActor* a : actors)
	{
		
		if (a->GetRootComponent()->ComponentHasTag("Ground"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, a->GetName());
			UStaticMeshComponent* sphereCol = FindComponentByClass<UStaticMeshComponent>();
			sphereCol->SetSimulatePhysics(false);
			sphereCol->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			sphereCol->SetEnableGravity(false);
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = GetInstigator();
			AActor* aTurret = GetWorld()->SpawnActor<AActor>(turret, GetActorLocation(), FRotator::ZeroRotator, spawnParams);
			spawned = true;
			FVector LineTraceEnd = FVector(FVector::DownVector * downCheck);
			FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
			FVector v = FVector(aTurret->GetActorLocation().X, aTurret->GetActorLocation().Y, aTurret->GetActorLocation().Z - turretHeight /*(bounds.GetBox().GetSize().Z)*/ - seedRadius / 2);
			aTurret->SetActorLocation(v);
		}
	}
	if (spawned)
	{
		
		Destroy();
	}
}

void ATurretSeed::OnHit(AActor* OverlappedActor, AActor* OtherActor)
{
	
}