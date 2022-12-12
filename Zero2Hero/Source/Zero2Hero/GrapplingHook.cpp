// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHook.h"

// Sets default values
AGrapplingHook::AGrapplingHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FireLocation = CreateDefaultSubobject<USphereComponent>(TEXT("Fire Location"));

	CableComp = CreateDefaultSubobject<UCableComponent>(TEXT("Cable Component"));

}

// Called when the game starts or when spawned
void AGrapplingHook::BeginPlay()
{
	Super::BeginPlay();

	CableComp->SetVisibility(false);

	CableComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called every frame
void AGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InUseHook != nullptr)
	{
		if (InUseHook->GetHookAttached())
		{
			isGrappling = false;
		}
	}

	if (!canGrapple)
	{
		//mag current location -> Hookpoint
		//if mag increases detach and 0 velocity

		float CurrentMag = (HookHit.GetActor()->GetActorLocation() - GetActorLocation()).Size();

		if (CurrentMag > PreviousMag + MagCheck)
		{
			if (!canGrapple)
			{
				Detach();
			}
		}

		PreviousMag = CurrentMag;
	}
}

void AGrapplingHook::Detach()
{
	isGrappling = false;
	EndGrapple = true;
	canGrapple = true;
}

bool AGrapplingHook::Fire()
{
	if (!isEnabled)
	{
		return false;
	}

	if (!canGrapple)
	{
		return false;
	}

	FVector LineTraceEnd = GrapplePoint->GetActorLocation();
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this->GetOwner());

	GetWorld()->LineTraceSingleByChannel(OUT HookHit, FireLocation->GetComponentLocation(), LineTraceEnd, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams());
	//DrawDebugLine(GetWorld(), GetActorLocation(), LineTraceEnd, FColor::Black, false, 1.0f, 0, 5);
	FVector dir;
	if (HookHit.IsValidBlockingHit())
	{
		if (HookHit.GetActor() != nullptr)
		{
			if (HookHit.GetActor()->ActorHasTag("GrapplePoint"))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Correct Tag"));
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;
				spawnParams.Instigator = GetInstigator();

				FRotator rotation = UKismetMathLibrary::FindLookAtRotation(FireLocation->GetComponentLocation(), HookHit.GetActor()->GetActorLocation());;

				GrappleShoot();

				InUseHook = GetWorld()->SpawnActor<AHook>(Hook, FireLocation->GetComponentLocation(), rotation, spawnParams);
				InUseHook->SetHookPointLocation(HookHit.ImpactPoint);

				isGrappling = true;
				PreviousMag = (GetActorLocation() - HookHit.GetActor()->GetActorLocation()).Size();
				EndGrapple = false;
				canGrapple = false;

				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("True"));
				return true;
			}
		}
	}
	if (HookHit.GetActor() != NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, HookHit.GetActor()->GetFName().ToString());
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Hit"));
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
	GrappleHit();
	return HookHit;
}

void AGrapplingHook::SetCamera(UCameraComponent* Camera)
{
	CameraComponent = Camera;
}

void AGrapplingHook::SetGrapplePoint(AActor* NewPoint)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Grapple"));
	GrapplePoint = NewPoint;

	if (GrapplePoint != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not Null"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Null"));
	}
}

AActor* AGrapplingHook::GetGrapplePoint()
{
	return GrapplePoint;
}

USphereComponent* AGrapplingHook::GetFireLocation()
{
	return FireLocation;
}

//void AGrapplingHook::CableOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor->ActorHasTag("GrapplePoint"))
//	{
//		//isGrappling = false;
//	}
//}

bool AGrapplingHook::GetIsGrappling()
{
	return isGrappling;
}

void AGrapplingHook::SetIsGrappling(bool newGrappling)
{
	canGrapple = newGrappling;
}

UCableComponent* AGrapplingHook::GetCable()
{
	return CableComp;
}

void AGrapplingHook::OnHit(AActor* OverlappedActor, AActor* OtherActor)
{

}

AHook* AGrapplingHook::GetInUseHook()
{
	return InUseHook;
}

bool AGrapplingHook::GetEndGrapple()
{
	return EndGrapple;
}

void AGrapplingHook::SetEndGrapple(bool newGrapple)
{
	EndGrapple = newGrapple;
}

bool AGrapplingHook::GetCanGrapple()
{
	return canGrapple;
}

