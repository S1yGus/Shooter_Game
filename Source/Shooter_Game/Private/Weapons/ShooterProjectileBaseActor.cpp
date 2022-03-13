// Shooter_Game, All rights reserved.

#include "Weapons/ShooterProjectileBaseActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapons/ShooterBaseWeaponActor.h"
#include "Weapons/Components/ShooterWeaponFXComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "Kismet/GameplayStatics.h"

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel3

AShooterProjectileBaseActor::AShooterProjectileBaseActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SphereComponent->SetSphereRadius(5.0f);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionObjectType(ECC_Projectile);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SphereComponent->SetCollisionResponseToChannel(ECC_Projectile, ECollisionResponse::ECR_Ignore);
    SphereComponent->bReturnMaterialOnMove = true;
    SetRootComponent(SphereComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovment");
    ProjectileMovement->InitialSpeed = 10000.0f;
    ProjectileMovement->ProjectileGravityScale = 0.3f;
}

void AShooterProjectileBaseActor::BeginPlay()
{
    Super::BeginPlay();

    check(SphereComponent);
    check(ProjectileMovement);

    SphereComponent->OnComponentHit.AddDynamic(this, &AShooterProjectileBaseActor::OnProjectileHit);
    // SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
    ProjectileMovement->Velocity = ShotDirection * ProjectileMovement->InitialSpeed;

    SetLifeSpan(LifeSpan);
}

void AShooterProjectileBaseActor::OnProjectileHit(UPrimitiveComponent* HitComponent,    //
                                                  AActor* OtherActor,                   //
                                                  UPrimitiveComponent* OtherComp,       //
                                                  FVector NormalImpulse,                //
                                                  const FHitResult& Hit)
{
    if (!Hit.GetActor())
        return;

    ProjectileMovement->StopMovementImmediately();

    FPointDamageEvent DamageEvent;
    DamageEvent.HitInfo = Hit;
    Hit.GetActor()->TakeDamage(DamageAmount, DamageEvent, GetPawnController(), this);

    MakeImpactFX(Hit);

    Destroy();
}

void AShooterProjectileBaseActor::MakeImpactFX(const FHitResult& HitResult)
{
    const auto WeaponFXComponent = GetOwner()->FindComponentByClass<UShooterWeaponFXComponent>();
    if (!WeaponFXComponent)
        return;

    WeaponFXComponent->MakeImpactFX(HitResult);
}

AController* AShooterProjectileBaseActor::GetPawnController() const
{
    const auto Pawn = GetOwner()->GetOwner<APawn>();
    return Pawn ? Pawn->GetController() : nullptr;
}
