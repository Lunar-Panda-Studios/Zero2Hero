// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Player Radius"));
	PlayerRadius->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	MainBody = FindComponentByClass<UCapsuleComponent>();

	PlayerRadius->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	PlayerRadius->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);

	//MainBody->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnMainBodyHit);

	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			InRange = true;
		}
	}
}

void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			InRange = false;
		}
	}
}

void AEnemy::OnMainBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			if (OtherComp->ComponentHasTag("MeleeZone"))
			{
				//DAMAGE SELF;
			}
		}
	}
}


