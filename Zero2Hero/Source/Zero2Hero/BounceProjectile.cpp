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
	//AActor* Player = World->GetFirstPlayerController()->GetPawn();

	//FVector AimedLocation = Player->GetActorLocation();

	FRotator OtherObject = OtherActor->GetActorRotation();
	FRotator Aimed = GetActorRotation() * -1;

	//float Difference = OtherObject.Yaw - Aimed.Yaw;

	//if (OtherObject.Yaw - 90 > Aimed.Yaw)
	//{

	//}

	//Aimed = FRotator(Aimed.Pitch, Aimed.Roll - SplitAngle/(NoSplitInto/2), Aimed.Yaw);

	for (int i = 0; i < NoSplitInto; i++)
	{
		AProjectile* Projectile = World->SpawnActor<AProjectile>(SplitInto, GetActorLocation(), Aimed, spawnParams);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned"));

		//Aimed = FRotator(Aimed.Pitch, Aimed.Roll - SplitAngle, Aimed.Yaw);
	}
}

//void ABounceProjectile::OnComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (!OtherActor->ActorHasTag("Player"))
//	{
//		Split(OtherActor);
//	}
//}
