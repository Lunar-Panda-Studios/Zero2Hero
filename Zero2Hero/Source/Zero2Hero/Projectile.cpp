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

	MainBody = FindComponentByClass<UStaticMeshComponent>();
	//OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnHit);

	MainBody->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnComponentHit);

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
		if (isEnemyProjectile)
		{
			if (!OtherActor->ActorHasTag("Player"))
			{
				if (!OtherActor->ActorHasTag("Ignore"))
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
		else
		{
			if (!OtherActor->ActorHasTag("Enemy"))
			{
				if (!OtherActor->ActorHasTag("Ignore"))
				{
					if (!OtherActor->ActorHasTag("Projectile"))
					{
						if (OtherActor->ActorHasTag("Player"))
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
	}
}

void AProjectile::OnComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Actor Overlap"));
	if (OtherActor != nullptr)
	{
		if (!isEnemyProjectile)
		{
			if (!OtherActor->ActorHasTag("Player"))
			{
				if (!OtherActor->ActorHasTag("Ignore"))
				{
					if (!OtherActor->ActorHasTag("Projectile"))
					{
						if (!OtherComp->ComponentHasTag("Ignore"))
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
		}
		else
		{
			if (!OtherActor->ActorHasTag("Enemy"))
			{
				if (!OtherActor->ActorHasTag("Ignore"))
				{
					if (!OtherActor->ActorHasTag("Projectile"))
					{
						if (!OtherComp->ComponentHasTag("Ignore"))
						{
							if (OtherActor->ActorHasTag("Player"))
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
		}
	}
}

