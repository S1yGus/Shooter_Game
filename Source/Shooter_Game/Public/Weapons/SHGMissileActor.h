// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/SHGBaseProjectileActor.h"
#include "SHGMissileActor.generated.h"

UCLASS()
class SHOOTER_GAME_API ASHGMissileActor : public ASHGBaseProjectileActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", Meta = (ClampMin = "0"))
    float DamageRadius = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    bool bDoFullDamage = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    TEnumAsByte<ECollisionChannel> DamageTraceChannel = ECC_Visibility;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    TSubclassOf<UCameraShakeBase> ExplosionCameraShakeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    float ExplosionCameraShakeRadius = 1000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<AActor> SpawningOnHitClass;

    virtual void BeginPlay() override;

private:
    virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                 const FHitResult& Hit) override;

    inline void MakeExplosionCameraShake();
};
