// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyChaseMelee.h"

// Sets default values
AEnemyChaseMelee::AEnemyChaseMelee()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeleeCollider = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeCollider"));
	MeleeCollider->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AEnemyChaseMelee::BeginPlay()
{
	Super::BeginPlay();

	MeleeCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemyChaseMelee::OnOverlapMelee);
	MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called every frame
void AEnemyChaseMelee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InRange && AttackSpeedTimer == 0)
	{
		MeleeCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (InRange)
	{
		AttackSpeedTimer += DeltaTime;

		if (AttackSpeedTimer >= AttackSpeed)
		{
			AttackSpeedTimer = 0;
		}
	}

}

// Called to bind functionality to input
void AEnemyChaseMelee::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyChaseMelee::Attack()
{
}

void AEnemyChaseMelee::OnOverlapMelee(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, OverlappedComponent->GetReadableName());
		if (OtherComp->ComponentHasTag("MainBody"))
		{
			//DAMAGE PLAYER
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Player Damaged"));
		}
	}
}

