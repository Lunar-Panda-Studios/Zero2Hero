// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonFodder.h"

// Sets default values
ACannonFodder::ACannonFodder()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlastRadius = CreateDefaultSubobject<USphereComponent>(TEXT("BlastRadius"));
	BlastRadius->SetupAttachment(GetRootComponent());

	MeleeCollider = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeCollider"));
	MeleeCollider->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ACannonFodder::BeginPlay()
{
	Super::BeginPlay();

	BlastRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BlastRadius->OnComponentBeginOverlap.AddDynamic(this, &ACannonFodder::OnOverlapBeginExplode);

	if (MeleeCollider != nullptr)
	{
		MeleeCollider->OnComponentBeginOverlap.AddDynamic(this, &ACannonFodder::OnOverlapMelee);
		MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No Melee Collider - Cannon Fodder"));
	}
}

// Called every frame
void ACannonFodder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CanExplode)
	{
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

	if (InRange)
	{
		Timer += DeltaTime;

		if (Timer >= TimeToExplode)
		{
			Explode();
		}
	}
	else
	{
		Timer = 0;
	}

}

// Called to bind functionality to input
void ACannonFodder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACannonFodder::Explode()
{
	BlastRadius->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetLifeSpan(AnimationTimer);

}

void ACannonFodder::OnOverlapBeginExplode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		if (!OtherComp->ComponentHasTag("PlayerView"))
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
			Player->DecreaseHealth(Damage);

			FVector Direction = GetActorLocation() - Player->GetActorLocation();
			Direction.Normalize();

			Player->LaunchCharacter(Direction*KnockbackSpeed, true, true);
			
		}
	}
}

void ACannonFodder::OnOverlapMelee(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, OverlappedComponent->GetReadableName());
			if (OtherComp->ComponentHasTag("MainBody"))
			{

				ADamageable* otherDamageable = Cast<ADamageable>(OtherActor);
				otherDamageable->DecreaseHealth(GetDamage());
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Damage Player Melee"));
			}
		}
	}
}

