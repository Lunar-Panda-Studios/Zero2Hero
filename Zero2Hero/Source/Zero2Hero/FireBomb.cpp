
#include "FireBomb.h"
#include "Kismet\KismetSystemLibrary.h"

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
	sphere->OnComponentBeginOverlap.AddDynamic(this, &AFireBomb::OnHit);
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ignoreActors.Init(this, 1);
	seekClass = UStaticMesh::StaticClass();
}

// Called every frame
void AFireBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFireBomb::OnHit(UPrimitiveComponent* HitComp, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("thing"));
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


