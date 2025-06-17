// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/ProjectileActor.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Towers/MasterTower.h"

// Sets default values
AProjectileActor::AProjectileActor()
{
	bReplicates = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
	SetRootComponent(SphereCollision);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	ProjectileBody = CreateDefaultSubobject<UStaticMeshComponent>("Projectile Body");
	ProjectileBody->SetupAttachment(SphereCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bIsHomingProjectile = true;
}

// Called when the game starts or when spawned
void AProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectileActor::OnSphereOverlap);

	AMasterTower* projectileOwner = Cast<AMasterTower>(GetOwner());

	if (projectileOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is invalid in %s"), *GetName());
		return;
	}
	
	ProjectileMovement->HomingTargetComponent = projectileOwner->GetTargetedEnemy()->GetRootComponent();
}

void AProjectileActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}