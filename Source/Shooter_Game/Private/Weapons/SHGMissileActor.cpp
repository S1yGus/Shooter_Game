// Shooter_Game, All rights reserved.

#include "Weapons/SHGMissileActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SHGUtils.h"

constexpr static float MinimumDamage = 0.0f, InnerRadius = 0.0f;

void ASHGMissileActor::BeginPlay()
{
    Super::BeginPlay();

    check(ExplosionCameraShakeClass);
    check(DamageTraceChannel != ECollisionChannel::ECC_MAX);
}

void ASHGMissileActor::OnProjectileHit(UPrimitiveComponent* HitComponent,    //
                                       AActor* OtherActor,                   //
                                       UPrimitiveComponent* OtherComp,       //
                                       FVector NormalImpulse,                //
                                       const FHitResult& Hit)
{
    SHGUtils::ApplyRadialDamageWithImpulse(this,                                    //
                                           FMath::RandRange(Damage.X, Damage.Y),    //
                                           MinimumDamage,                           //
                                           ImpulseMultiplier,                       //
                                           GetActorLocation(),                      //
                                           InnerRadius,                             //
                                           DamageRadius,                            //
                                           false,                                   //
                                           UDamageType::StaticClass(),              //
                                           {},                                      //
                                           this,                                    //
                                           GetOwnerPawnController(),                //
                                           DamageTraceChannel);

    MakeExplosionCameraShake();

    if (SpawningOnHitClass)
    {
        GetWorld()->SpawnActor<AActor>(SpawningOnHitClass, Hit.ImpactPoint, FRotator::ZeroRotator);
    }
}

void ASHGMissileActor::MakeExplosionCameraShake()
{
    UGameplayStatics::PlayWorldCameraShake(this, ExplosionCameraShakeClass, GetActorLocation(), InnerRadius, ExplosionCameraShakeRadius);
}
