// Fill out your copyright notice in the Description page of Project Settings.


#include "Hook.h"

// Sets default values
AHook::AHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHook::BeginPlay()
{
	Super::BeginPlay();
	
	MainBody = FindComponentByClass<UStaticMeshComponent>();
	ProjectileMoveComp = FindComponentByClass<UProjectileMovementComponent>();

	if (MainBody != nullptr)
	{
		MainBody->OnComponentBeginOverlap.AddDynamic(this, &AHook::OnHit);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Main Body (Static Mesh Component) missing"));
	}

	PreviousMag = (HookPointLocation - GetActorLocation()).Size();
	
}

// Called every frame
void AHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentMag = (HookPointLocation - GetActorLocation()).Size();

	if (CurrentMag > PreviousMag + 10)
	{
		HookAttached = true;
		ProjectileMoveComp->Deactivate();
		SetLifeSpan(10);
	}

	PreviousMag = CurrentMag;

}

void AHook::SetHookPointLocation(FVector NewHookPointLocation)
{
	HookPointLocation = NewHookPointLocation;
}

UProjectileMovementComponent* AHook::GetMoveComp()
{
	return ProjectileMoveComp;
}

UStaticMeshComponent* AHook::GetMainBody()
{
	return MainBody;
}

bool AHook::GetHookAttached()
{
	return HookAttached;
}

void AHook::SetHookAttached(bool newAttached)
{
	HookAttached = newAttached;
}

void AHook::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherComp != nullptr)
		{
			if (OtherActor->ActorHasTag("GrapplePoint"))
			{
				if (OtherComp->ComponentHasTag(("MainBody")))
				{
					HookAttached = true;
					ProjectileMoveComp->Deactivate();
					SetLifeSpan(10);
				}
			}
		}
	}
}

