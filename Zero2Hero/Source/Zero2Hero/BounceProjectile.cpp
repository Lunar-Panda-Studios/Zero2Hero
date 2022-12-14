// Fill out your copyright notice in the Description page of Project Settings.


#include "BounceProjectile.h"

// Sets default values
ABounceProjectile::ABounceProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABounceProjectile::BeginPlay()
{
	Super::BeginPlay();

	//MainBody->OnComponentBeginOverlap.AddDynamic(this, &ABounceProjectile::OnComponentDestruction);

	MainBody->OnComponentHit.AddDynamic(this, &ABounceProjectile::OnComponentBodyHit);
	
}

// Called every frame
void ABounceProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABounceProjectile::Split(AActor* OtherActor)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	UWorld* World = GEngine->GameViewport->GetWorld();
	AActor* Player = World->GetFirstPlayerController()->GetPawn();

	FRotator newRoatation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());

	newRoatation = FRotator(newRoatation.Pitch, newRoatation.Yaw - (SplitAngle / (2)), newRoatation.Roll);

	for (int i = 0; i < NoSplitInto; i++)
	{
		AProjectile* Projectile = World->SpawnActor<AProjectile>(SplitInto, GetActorLocation(), newRoatation, spawnParams);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned"));

		newRoatation = FRotator(newRoatation.Pitch, newRoatation.Yaw + SplitAngle, newRoatation.Roll);
	}

	Destroy();
}

void ABounceProjectile::OnComponentBodyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr)
	{
		if (!OtherActor->ActorHasTag("Enemy") && !OtherActor->ActorHasTag("Projectile") && !OtherActor->ActorHasTag("Player"))
		{
			if (!OtherComp->ComponentHasTag("Ignore"))
			{
				Split(OtherActor);
			}
		}

		Destroy();
	}
}
