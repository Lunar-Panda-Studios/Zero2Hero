// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargeBolt.h"
#include "DrawDebugHelpers.h"
#include "Kismet\KismetSystemLibrary.h"

AChargeBolt::AChargeBolt()
{
	PrimaryActorTick.bCanEverTick = true;
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	NiagaraComp->SetupAttachment(GetRootComponent());
}

void AChargeBolt::BeginPlay()
{
	Super::BeginPlay();
	NiagaraComp = FindComponentByClass<UNiagaraComponent>();
	NiagaraComp->SetAsset(ElectricSystem);
}

void AChargeBolt::OnHit(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		if (NiagaraComp != nullptr)
		{
			NiagaraComp->ActivateSystem();
		}
		CheckArea(OtherActor->GetActorLocation());
	}
}

void AChargeBolt::CheckArea(FVector pos)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ignoreActors;
	TArray<AActor*> actorsHit;
	UClass* seekClass = ACharacter::StaticClass();
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), pos, chargeRadius, traceObjectTypes, seekClass, ignoreActors, actorsHit);

	for (AActor* i : actorsHit)
	{
		if (i->ActorHasTag("Enemy"))
		{
			AEnemy* enemy = Cast<AEnemy>(i); //this is dangerous. If anything is given the enemy tag that isnt AEnemy the game will crash
			ConnectCharge(enemy);
		}
	}
}

void AChargeBolt::OnParticleHit(AEnemy* Enemy)
{
}

void AChargeBolt::ConnectCharge(AEnemy* Enemy)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = GetInstigator();
	FRotator rotation = GetActorRotation();
	AActor* connectorActor = GetWorld()->SpawnActor<AActor>(ConnectionNiagaraSystem, Enemy->GetActorLocation(), rotation, spawnParams);
	++currentEnemiesHit;
	if (currentEnemiesHit >= maxEnemiesHit)
	{
		Destroy();
	}
	else
	{
		CheckArea(Enemy->GetActorLocation());
	}
}
