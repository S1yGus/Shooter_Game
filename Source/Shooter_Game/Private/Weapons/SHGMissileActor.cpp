// Shooter_Game, All rights reserved.

#include "Weapons/SHGMissileActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void ASHGMissileActor::BeginPlay()
{
    Super::BeginPlay();

    check(ExplosionCameraShakeClass);
}

void ASHGMissileActor::OnProjectileHit(UPrimitiveComponent* HitComponent,    //
                                       AActor* OtherActor,                   //
                                       UPrimitiveComponent* OtherComp,       //
                                       FVector NormalImpulse,                //
                                       const FHitResult& Hit)
{
    ProjectileMovement->StopMovementImmediately();

    UGameplayStatics::ApplyRadialDamage(this,                                    //
                                        FMath::RandRange(Damage.X, Damage.Y),    //
                                        GetActorLocation(),                      //
                                        DamageRadius,                            //
                                        UDamageType::StaticClass(),              //
                                        {},                                      //
                                        this,                                    //
                                        GetController(),                         //
                                        DoFullDamage);

    MakeImpactFX(Hit);
    MakeExplosionCameraShake();

    Destroy();
}

void ASHGMissileActor::MakeExplosionCameraShake()
{
    UGameplayStatics::PlayWorldCameraShake(this, ExplosionCameraShakeClass, GetActorLocation(), 0, ExplosionCameraShakeRadius);
}
