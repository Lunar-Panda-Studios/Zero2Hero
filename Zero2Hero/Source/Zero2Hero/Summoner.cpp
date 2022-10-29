// Fill out your copyright notice in the Description page of Project Settings.


#include "Summoner.h"

// Sets default values
ASummoner::ASummoner()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SummonRange = CreateDefaultSubobject<USphereComponent>(TEXT("SummonRange"));
	SummonRange->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ASummoner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASummoner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InRange)
	{
		TimerToSummon += DeltaTime;

		if (TimerToSummon >= TimeToSummon)
		{
			Summon();
			TimerToSummon = 0;
			CanSummon = false;
		}
	}

	if (!CanSummon)
	{
		CooldownTimer += DeltaTime;

		if (CooldownTimer >= SummonCooldown)
		{
			CanSummon = true;
			CooldownTimer = 0;
		}
	}

}

// Called to bind functionality to input
void ASummoner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASummoner::Summon()
{
	float Chance = FMath::FRandRange(0, 100);

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	UWorld* World = GEngine->GameViewport->GetWorld();

	FVector RandLocation;

		if (Chance >= ChanceOfSpawningStrong)
		{
			for (int i = 0; i < AmountToSummon; i++)
			{
				RandLocation = FVector(GetActorLocation().X + FMath::RandRange(-SummonRange->GetScaledSphereRadius(), SummonRange->GetScaledSphereRadius()), 
					GetActorLocation().Y + FMath::RandRange(-SummonRange->GetScaledSphereRadius(), SummonRange->GetScaledSphereRadius()),
					GetActorLocation().Z);

				AEnemy* Spawned = World->SpawnActor<AEnemy>(EnemyToSpawn, RandLocation, GetActorRotation(), spawnParams);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned Weak"));
			}
		}
		else
		{
			RandLocation = FVector(FMath::RandRange(-SummonRange->GetScaledSphereRadius(), SummonRange->GetScaledSphereRadius()), FMath::RandRange(-SummonRange->GetScaledSphereRadius(), SummonRange->GetScaledSphereRadius()), FMath::RandRange(-SummonRange->GetScaledSphereRadius(), SummonRange->GetScaledSphereRadius()));

			AEnemy* Spawned = World->SpawnActor<AEnemy>(StrongerEnemyToSpawn, RandLocation, GetActorRotation(), spawnParams);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned Strong"));
		}
}

float ASummoner::GetRunAwayDistance()
{
	return RunAwayDistance;
}

