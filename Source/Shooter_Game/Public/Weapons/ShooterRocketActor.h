// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ShooterProjectileBaseActor.h"
#include "ShooterRocketActor.generated.h"

UCLASS()
class SHOOTER_GAME_API AShooterRocketActor : public AShooterProjectileBaseActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<UCameraShakeBase> CameraShakeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    float ExplosionCameraShakeRadius = 1000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile", Meta = (ClampMin = "0"))
    float DamageRadius = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    bool DoFullDamage = false;

private:
    virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                 const FHitResult& Hit) override;

    void MakeExplosionCameraShake();
};
