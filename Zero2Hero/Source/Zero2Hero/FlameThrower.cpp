// Fill out your copyright notice in the Description page of Project Settings.


#include "FlameThrower.h"

// Sets default values
AFlameThrower::AFlameThrower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	NiagaraComp->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AFlameThrower::BeginPlay()
{
	Super::BeginPlay();

	NiagaraComp = FindComponentByClass<UNiagaraComponent>();
	NiagaraComp->SetAsset(NigaraSys);

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

	if (InUse)
	{
		Timer += DeltaTime;
		if (TimerMax < Timer)
		{
			Timer = 0.0f;
			if (!DecreaseCharge(ChargeUsage))
			{
				PrimaryAttackEnd();
			}
		}

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(Charge));
	}

}

void AFlameThrower::PrimaryAttack()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Active Flamethrower"));
	InUse = true;

	if (NiagaraComp != nullptr)
	{
		if (DecreaseCharge(ChargeUsage))
		{
			NiagaraComp->ActivateSystem();
		}
	}
}

void AFlameThrower::OnParticleHit(AEnemy* Enemy)
{
	if (Enemy != nullptr)
	{
		Enemy->SetOnFire(true);
		Enemy->SetFlameDamage(Damage);
		Enemy->DecreaseHealth(Damage);
	}
}

void AFlameThrower::PrimaryAttackEnd()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Deactive Flamethrower"));

	InUse = false;

	if (NiagaraComp != nullptr)
	{
		NiagaraComp->Deactivate();
	}

	Timer = 0.0f;
}

void AFlameThrower::SecondaryAttack()
{

}

