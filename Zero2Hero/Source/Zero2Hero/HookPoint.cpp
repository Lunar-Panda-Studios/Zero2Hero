// Fill out your copyright notice in the Description page of Project Settings.


#include "HookPoint.h"

// Sets default values
AHookPoint::AHookPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpottingRange = CreateDefaultSubobject<USphereComponent>(TEXT("Spotting Range"));
	SpottingRange->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AHookPoint::BeginPlay()
{
	Super::BeginPlay();

	SpottingRange->OnComponentBeginOverlap.AddDynamic(this, &AHookPoint::OnOverlapRangeSphere);
	SpottingRange->OnComponentEndOverlap.AddDynamic(this, &AHookPoint::OnOverlapRangeSphereEnd);
}

// Called every frame
void AHookPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHookPoint::OnOverlapRangeSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OverLap"));

	if (OtherComp != nullptr)
	{
		if (OtherComp->ComponentHasTag("PlayerView"))
		{
			if (GrapplingHook != nullptr)
			{
				InSight = true;
				GrapplingHook->SetGrapplePoint(this);
			}
		}
	}
}

void AHookPoint::OnOverlapRangeSphereEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("EndOverlap"));
	if (OtherComp != nullptr)
	{
		if (OtherComp->ComponentHasTag("PlayerView"))
		{
			if (GrapplingHook != nullptr)
			{
				InSight = false;
				GrapplingHook->SetGrapplePoint(nullptr);
			}
		}
	}
}

void AHookPoint::SetGrapplingHook(AGrapplingHook* NewGrapple)
{
	GrapplingHook = NewGrapple;
}

