// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeapon.h"
#include "Kismet\KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARangedWeapon::ARangedWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//FireLocation = FindComponentByClass<USphereComponent>();

}

// Called when the game starts or when spawned
void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();

	Charge = MaxCharge;
	CurrentAmmo = AmmoMax;
}

// Called every frame
void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Reload())
	{
		SetTimerReload(GetTimerReload() + DeltaTime);

		if (GetTimerReload() >= GetTimeToReload())
		{
			SetTimerReload(0);
			AmmoCheck();
			Charge = MaxCharge;
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Tick"));

}

void ARangedWeapon::PrimaryAttack()
{

}

void ARangedWeapon::PrimaryAttackEnd()
{
}

void ARangedWeapon::SecondaryAttack()
{
}

int ARangedWeapon::GetCharge()
{
	return Charge;
}

void ARangedWeapon::IncreaseCharge(int amount)
{
	Charge += amount;
}

bool ARangedWeapon::DecreaseCharge(int amount)
{
	if (!isEnabled)
	{
		return false;
	}
	if (Charge - amount < 0)
	{
		return false;
	}

	Charge -= amount;
	return true;
}

int ARangedWeapon::GetUsage()
{
	return ChargeUsage;
}

float ARangedWeapon::GetAmmo()
{
	return CurrentAmmo;
}

void ARangedWeapon::IncreaseAmmo(int amount)
{
	CurrentAmmo += amount;
}

void ARangedWeapon::DecreaseAmmo(int amount)
{
	CurrentAmmo -= amount;
}

bool ARangedWeapon::AmmoCheck()
{
	if (CurrentAmmo - 1 >= 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Pass Ammo Check"));
		CurrentAmmo -= 1;
		return true;
	}

	return false;
}

bool ARangedWeapon::Reload()
{
	if (Charge - ChargeUsage < 0)
	{
		if (CurrentAmmo == 0)
		{
			return false;
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Reload"));
		Reloading();
		return true;
	}

	return false;
}

float ARangedWeapon::GetTimerReload()
{
	return TimerReload;
}

void ARangedWeapon::SetTimerReload(float amount)
{
	TimerReload += amount;
}

float ARangedWeapon::GetTimeToReload()
{
	return TimeToReload;
}

ACamera* ARangedWeapon::GetCamera()
{
	return Camera;
}

void ARangedWeapon::SetCamera(ACamera* newCamera)
{
	Camera = newCamera;
}

FName ARangedWeapon::GetWeaponName()
{
	return WeaponTypeName;
}

void ARangedWeapon::SetAmmo(float ammo)
{
	CurrentAmmo = ammo;
}

FRotator ARangedWeapon::spawnRot()
{
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	AActor* p = GetWorld()->GetFirstPlayerController();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Player->GetName());
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(p);
	TraceParams.AddIgnoredActor(Player);
	TArray<AActor*> outChildren;
	GetWorld()->GetFirstPlayerController()->GetAttachedActors(outChildren);
	TraceParams.AddIgnoredActors(outChildren);
	TraceParams.AddIgnoredActor(Camera);
	FHitResult Hit;
	UCameraComponent* cameraComp = Camera->GetCameraComp();
	FVector loc = cameraComp->GetComponentLocation();
	FVector x = FVector(cameraComp->GetForwardVector() * cameraRayRange + loc);
	if (GetWorld()->LineTraceSingleByChannel(OUT Hit, loc, x, ECollisionChannel::ECC_WorldStatic, TraceParams, FCollisionResponseParams()))
	{
		return UKismetMathLibrary::FindLookAtRotation(FireLocation->GetComponentLocation(), Hit.ImpactPoint);
	}
	return FRotator(0, 0, 0);
}

