// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	MainBody = FindComponentByClass<UCapsuleComponent>();
	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnHit);

	SetLifeSpan(10);

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(AActor* OverlappedActor, AActor* OtherActor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Actor Overlap"));
	if (OtherActor != nullptr)
	{
		if (!OtherActor->ActorHasTag("Player"))
		{
			if (!OtherActor->ActorHasTag("Projectile"))
			{
				if (OtherActor->ActorHasTag("Enemy"))
				{
					ADamageable* OtherDamageable = Cast<ADamageable>(OtherActor);
					OtherDamageable->DecreaseHealth(Damage);

					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Damage Enemy Projectile"));
				}
				Destroy();
			}
		}
	}
}

