// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHook.h"

// Sets default values
AGrapplingHook::AGrapplingHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CameraComponent = FindComponentByClass<UCameraComponent>();

	FireLocation = CreateDefaultSubobject<USphereComponent>(TEXT("Fire Location"));
	//FireLocation->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AGrapplingHook::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGrapplingHook::Fire()
{
	FVector LineTraceEnd = (CameraComponent->GetForwardVector() * Range) + FireLocation->GetComponentLocation();
	//FVector Start = FireLocation->GetComponentLocation();
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this->GetOwner());

	GetWorld()->LineTraceSingleByChannel(OUT HookHit, FireLocation->GetComponentLocation(), LineTraceEnd, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams());
	DrawDebugLine(GetWorld(), GetActorLocation(), LineTraceEnd, FColor::Black, false, 1.0f, 0, 5);
	FVector dir;
	if (HookHit.IsValidBlockingHit())
	{
		if (HookHit.GetActor()->ActorHasTag("GrapplePoint"))
		{
			return true;
		}
	}

	return false;
}

bool AGrapplingHook::HookReturned()
{
	if (GetActorLocation() == FireLocation->GetComponentLocation())
	{
		return true;
	}

	return false;
}

FHitResult AGrapplingHook::GetHit()
{
	return HookHit;
}

void AGrapplingHook::SetCamera(UCameraComponent* Camera)
{
	CameraComponent = Camera;
}

