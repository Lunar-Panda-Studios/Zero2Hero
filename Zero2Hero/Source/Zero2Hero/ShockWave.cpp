// Fill out your copyright notice in the Description page of Project Settings.


#include "ShockWave.h"

// Sets default values
AShockWave::AShockWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShockWave::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> comps = GetComponentsByTag(UStaticMeshComponent::StaticClass(), TEXT("OuterMesh"));
	MeshOuter = Cast<UStaticMeshComponent>(comps[0]);

	comps = GetComponentsByTag(UStaticMeshComponent::StaticClass(), TEXT("InnerMesh"));
	MeshInner = Cast<UStaticMeshComponent>(comps[0]);

	MeshOuter->OnComponentBeginOverlap.AddDynamic(this, &AShockWave::OnOverlapOuter);

	MeshInner->OnComponentBeginOverlap.AddDynamic(this, &AShockWave::OnOverlapInner);
	MeshInner->OnComponentEndOverlap.AddDynamic(this, &AShockWave::OnOverlapInnerEnd);

	SetLifeSpan(TimeToLive);
	
}

// Called every frame
void AShockWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorScale3D(FVector(GetActorScale().X + (Speed*DeltaTime), GetActorScale().Y + (Speed * DeltaTime), GetActorScale().Z));

}

void AShockWave::OnOverlapInner(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
	{
		return;
	}

	if (OtherComp == nullptr)
	{
		return;
	}

	if (OtherActor->ActorHasTag("Player"))
	{
		if (!OtherComp->ComponentHasTag("Ignore"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("No Damage"));
			CanDamage = false;
		}
	}
}

void AShockWave::OnOverlapInnerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr)
	{
		return;
	}

	if (OtherComp == nullptr)
	{
		return;
	}

	if (OtherActor->ActorHasTag("Player"))
	{
		if (!OtherComp->ComponentHasTag("Ignore"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Can Damage"));
			CanDamage = true;
		}
	}
}

void AShockWave::OnOverlapOuter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CanDamage)
	{
		return;
	}

	if (OtherActor == nullptr)
	{
		return;
	}

	if (OtherComp == nullptr)
	{
		return;
	}

	if (OtherActor->ActorHasTag("Player"))
	{
		if (!OtherComp->ComponentHasTag("Ignore"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Take Damage"));
			Cast<ADamageable>(GetWorld()->GetFirstPlayerController()->GetPawn())->DecreaseHealth(Damage);
		}
	}
}

