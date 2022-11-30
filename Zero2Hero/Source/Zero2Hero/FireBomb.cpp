
#include "FireBomb.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Enemy.h"

// Sets default values
AFireBomb::AFireBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFireBomb::BeginPlay()
{
	Super::BeginPlay();
	sphere = FindComponentByClass<USphereComponent>();
	sphere->OnComponentHit.AddDynamic(this, &AFireBomb::OnHit);
	
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ignoreActors.Init(this, 1);
	seekClass = UStaticMesh::StaticClass();
}

// Called every frame
void AFireBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFireBomb::OnHit(UPrimitiveComponent* HitComp, class AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const FHitResult& Hit)
{
	if (OtherActor == NULL)
	{
		return;
	}
	if (!OtherActor->ActorHasTag("Player"))
	{
		if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), radius, traceObjectTypes, AActor::StaticClass(), ignoreActors, actors))
		{
			for (AActor* a : actors)
			{
				if (Cast<AEnemy>(a))
				{
					Cast<AEnemy>(a)->DecreaseHealth(damage);
				}
			}
		}
		Destroy();

	}
}


