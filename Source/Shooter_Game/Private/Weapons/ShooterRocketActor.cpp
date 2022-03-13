// Shooter_Game, All rights reserved.

#include "Weapons/ShooterRocketActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapons/Components/ShooterWeaponFXComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void AShooterRocketActor::OnProjectileHit(UPrimitiveComponent* HitComponent,    //
                                          AActor* OtherActor,                   //
                                          UPrimitiveComponent* OtherComp,       //
                                          FVector NormalImpulse,                //
                                          const FHitResult& Hit)
{
    if (!Hit.GetActor())
        return;

    ProjectileMovement->StopMovementImmediately();

    UGameplayStatics::ApplyRadialDamage(GetWorld(),                    //
                                        DamageAmount,                  //
                                        GetActorLocation(),            //
                                        DamageRadius,                  //
                                        UDamageType::StaticClass(),    //
                                        {},                            //
                                        this,                          //
                                        GetPawnController(),               //
                                        DoFullDamage);
    // DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 20, FColor::Purple, false, 2.0f);

    MakeImpactFX(Hit);
    MakeExplosionCameraShake();

    Destroy();
}

void AShooterRocketActor::MakeExplosionCameraShake()
{
    if (!CameraShakeClass)
        return;

    UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeClass, GetActorLocation(), 0, ExplosionCameraShakeRadius);
}