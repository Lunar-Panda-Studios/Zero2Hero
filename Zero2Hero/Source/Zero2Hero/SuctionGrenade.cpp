// Fill out your copyright notice in the Description page of Project Settings.
#include "SuctionGrenade.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Damageable.h"
#include "Enemy.h"

// Sets default values
ASuctionGrenade::ASuctionGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASuctionGrenade::BeginPlay()
{
	Super::BeginPlay();
	currentSuccTime = 0;

	seekClass = AActor::StaticClass();
	ignoreActors.Init(this, 1);
}

// Called every frame
void ASuctionGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isSucking)
	{
		currentExplodeTime += DeltaTime;
	}
	
	if (currentExplodeTime > explodeTime)
	{
		USphereComponent* sphereCol = FindComponentByClass<USphereComponent>();
		sphereCol->SetSimulatePhysics(false);
		sphereCol->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		sphereCol->SetEnableGravity(false);
		isSucking = true;
	}

	FVector dir;
	if (isSucking)
	{
		currentSuccTime += DeltaTime;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), succRadius, traceObjectTypes, seekClass, ignoreActors, actors);
		for (AActor* a : actors)
		{
			if (Cast<AEnemy>(a))
			{
				if (Cast<AEnemy>(a)->ActorHasTag("Boss"))
				{
					continue;
				}

				//for some reason this sometimes doesnt work with the follow enemy. Sometimes it does, but when it does it doesnt work
				// for the whole time. Dont get it. There are no (major) issues with my code. so confused
				dir = GetActorLocation() - a->GetActorLocation();
				dir.Normalize();
				dir = dir * succSpeed;
				DamageableTarget = Cast<ADamageable>(a);
				DamageableTarget->LaunchCharacter(dir, false, false);
				/*DamageableTarget = a->FindComponentByClass<UStaticMeshComponent>();
				DamageableTarget->AddForce(dir);*/
			}
		}
	}
	if (isSucking && currentSuccTime > succTime)
	{
		isSucking = false;
		currentSuccTime = 0.0f;
		Destroy();
	}
}

