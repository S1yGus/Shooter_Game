// Shooter_Game, All rights reserved.


#include "Weapons/ShooterProjectileBaseActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AShooterProjectileBaseActor::AShooterProjectileBaseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SphereComponent->SetSphereRadius(5.0f);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SetRootComponent(SphereComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovment");
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->ProjectileGravityScale = 0.3f;
}

void AShooterProjectileBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	check(ProjectileMovement);
    check(SphereComponent);
    check(CameraShakeClass);

	SphereComponent->OnComponentHit.AddDynamic(this, &AShooterProjectileBaseActor::OnProjectileHit);
    //SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
	ProjectileMovement->Velocity = ShotDirection * ProjectileMovement->InitialSpeed;

	SetLifeSpan(LifeSpan);
}

void AShooterProjectileBaseActor::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                                  const FHitResult& Hit)
{
    ProjectileMovement->StopMovementImmediately();
    UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GetActorLocation(), DamageRadius, UDamageType::StaticClass(), {}, this, GetController(), DoFullDamage);
    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 20, FColor::Purple, false, 2.0f);
    MakeCameraShake();

    Destroy();
}

AController* AShooterProjectileBaseActor::GetController() const
{
    APawn* Pawn = Cast<APawn>(GetOwner());

    return Pawn ? Pawn->GetController() : nullptr;
}

void AShooterProjectileBaseActor::MakeCameraShake()
{
    if (!CameraShakeClass)
        return;

    UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeClass, GetActorLocation(), 0, DamageRadius * 10);
}
