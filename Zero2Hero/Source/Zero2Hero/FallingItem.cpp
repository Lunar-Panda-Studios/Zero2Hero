// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingItem.h"

// Sets default values
AFallingItem::AFallingItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFallingItem::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &AFallingItem::OnHit);
	
}

// Called every frame
void AFallingItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFallingItem::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr)
	{
		return;
	}

	if (OtherActor->ActorHasTag("Ground"))
	{
		ReachedGround = true;
	}

	if (OtherActor->ActorHasTag("Boss"))
	{
		Destroy();
	}
}

