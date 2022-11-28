// Fill out your copyright notice in the Description page of Project Settings.


#include "FlameThrower.h"

// Sets default values
AFlameThrower::AFlameThrower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponTypeName = "Fire";
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	NiagaraComp->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AFlameThrower::BeginPlay()
{
	Super::BeginPlay();
	FireLocation = FindComponentByClass<USphereComponent>();

	/*NiagaraComp = FindComponentByClass<UNiagaraComponent>();
	NiagaraComp->SetAsset(NigaraSys);*/
	WeaponType = 0;
	Timer = TimerMax;

	//if (NiagaraComp != nullptr)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Has Reference"));
	//	NiagaraComp->Deactivate();
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Reference"));
	//}
	
}

// Called every frame
void AFlameThrower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timer += DeltaTime;

	if (shoot)
	{
		currentTimeToThrow += DeltaTime;
		if (currentTimeToThrow > timeToThrow)
		{
			shoot = false;
			currentTimeToThrow = 0.0f;
			Fire();
		}
	}

	//if (InUse)
	//{
	//	Timer += DeltaTime;
	//	if (TimerMax < Timer)
	//	{
	//		Timer = 0.0f;
	//		if (!DecreaseCharge(ChargeUsage))
	//		{
	//			PrimaryAttackEnd();
	//		}
	//	}

	//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(Charge));
	//}

}

void AFlameThrower::PrimaryAttack()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Active Flamethrower"));
	/*InUse = true;

	if (NiagaraComp != nullptr)
	{
		if (DecreaseCharge(ChargeUsage))
		{
			NiagaraComp->ActivateSystem();
		}
	}*/
	if (Timer > TimerMax && DecreaseCharge(ChargeUsage))
	{
		shoot = true;
	}
}

void AFlameThrower::OnParticleHit(AEnemy* Enemy)
{
	/*if (Enemy != nullptr)
	{
		Enemy->SetOnFire(true);
		Enemy->SetFlameDamage(Damage);
		Enemy->DecreaseHealth(Damage);
	}*/
}

void AFlameThrower::PrimaryAttackEnd()
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Deactive Flamethrower"));

	InUse = false;

	if (NiagaraComp != nullptr)
	{
		NiagaraComp->Deactivate();
	}

	Timer = 0.0f;*/
}

void AFlameThrower::SecondaryAttack()
{
	
}

void AFlameThrower::Fire()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();
	FRotator rotation = Camera->GetSpringArm()->GetComponentRotation();
	rotation.Pitch += CameraAimDifference;
	rotation.Yaw += CameraAimDifferenceYaw;

	AFireBomb* fb = GetWorld()->SpawnActor<AFireBomb>(FireBomb, FireLocation->GetComponentLocation(), rotation, spawnParams);

	/*FVector Dir = FVector(GetActorRotation().Vector() * launchSpeed) + FVector(0, 0, launchSpeed);*/
	USphereComponent* sphereCol = fb->FindComponentByClass<USphereComponent>();
	if (sphereCol)
	{
		sphereCol->AddImpulse(GetActorForwardVector() * launchSpeed);
	}
	Timer = 0;
}

