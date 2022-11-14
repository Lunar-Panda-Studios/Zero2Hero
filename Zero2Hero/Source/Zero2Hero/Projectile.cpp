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

	MainBody->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnComponentOverlap);
	//MainBody->OnComponentHit.AddDynamic(this, &AProjectile::OnComponentHit);

	SetLifeSpan(10);

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlap"));
	if (OtherActor == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not Actor Overlapping"));
		return;
	}

	if (OtherComp->ComponentHasTag("PlayerView"))
	{
		return;
	}
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, OtherActor->GetFName().ToString());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, OtherComp->GetFName().ToString());*/

	if (!isEnemyProjectile)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			return;
		}

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("!Player"));
		if (OtherActor->ActorHasTag("Ignore"))
		{
			return;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("!Ignore"));
		if (OtherActor->ActorHasTag("Projectile"))
		{
			return;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("!Projectile"));
		if (OtherComp->ComponentHasTag("Ignore"))
		{
			return;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("!Ignore"));
		if (OtherActor->ActorHasTag("Enemy"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Has Tag Enemy"));
			ADamageable* OtherDamageable = Cast<ADamageable>(OtherActor);

			if (OtherDamageable->GetIsShielded())
			{
				if (!OtherDamageable->GetIsShieldReflect())
				{
					if (OtherDamageable->GetShieldType() == ElementType)
					{
						OtherDamageable->DecreaseHealth(Damage);
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Through regular shield"));
					}
				}
				else
				{
					if (OtherDamageable->GetShieldType() != ElementType)
					{
						OtherDamageable->DecreaseHealth(Damage);
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Through reflect shield"));
					}
					else
					{
						UWorld* World = GEngine->GameViewport->GetWorld();
						ADamageable* Player = Cast<ADamageable>(World->GetFirstPlayerController()->GetPawn());

						Player->DecreaseHealth(Damage);

						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Recoil Damage"));

					}
				}
			}
			else
			{
				OtherDamageable->DecreaseHealth(Damage);

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Damage Enemy Projectile"));
			}
		}

		HasDestruct = true;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Destroy Projectil"));
		Destroy();						
	}
	else
	{
		if (OtherActor->ActorHasTag("Enemy"))
		{
			return;
		}
		if (OtherActor->ActorHasTag("Ignore"))
		{
			return;
		}
		if (OtherActor->ActorHasTag("Projectile"))
		{
			return;
		}
		if (!OtherComp->ComponentHasTag("Ignore"))
		{
			if (OtherActor->ActorHasTag("Player"))
			{
				ADamageable* OtherDamageable = Cast<ADamageable>(OtherActor);
				OtherDamageable->DecreaseHealth(Damage);

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Damage Enemy Projectile"));
			}
			HasDestruct = true;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Destroy Projectil"));
			Destroy();
		}
	}
}

bool AProjectile::GetHasDestruct()
{
	return HasDestruct;
}
