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
	//GEngine->ClearOnScreenDebugMessages();
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
		
		electricutedEnemies.Add(Cast<AEnemy>(OtherActor));
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
	AEnemy* closest = nullptr;
	for (AActor* i : actorsHit)
	{ //need to make sure that already electricuted enemies arent included in this
		if (i->ActorHasTag("Enemy"))
		{
			if (closest != nullptr)
			{
				//this sucks ass
				bool isSame = false;
				
				for (AEnemy* e : electricutedEnemies)
				{
					if (e == Cast<AEnemy>(i))
					{
						isSame = true;
					}
				}
				if (FVector::Dist(pos, i->GetActorLocation()) > FVector::Dist(pos, closest->GetActorLocation()) && !isSame)
				{
					closest = Cast<AEnemy>(i);//this is dangerous. If anything is given the enemy tag that isnt AEnemy the game will crash
				}
			}
			else
			{
				closest = Cast<AEnemy>(i);
			}
		}
	}
	if (closest != nullptr)
	{
		electricutedEnemies.Add(closest);
		ConnectCharge(closest);
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
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, Enemy->GetName());
	
	if (currentEnemiesHit >= maxEnemiesHit)
	{
		Destroy();
	}
	else
	{
		CheckArea(Enemy->GetActorLocation());
	}
}
