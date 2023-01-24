// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHGFXCoreTypes.h"
#include "SHGBaseProjectileActor.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UPhysicalMaterial;

UCLASS()
class SHOOTER_GAME_API ASHGBaseProjectileActor : public AActor
{
    GENERATED_BODY()

public:
    ASHGBaseProjectileActor();

    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USphereComponent* HitSphereComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    FVector2D Damage{0.0f, 0.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
    float ImpulseMultiplier = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FImpactFXData DefaultImpactFXData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    TMap<UPhysicalMaterial*, FImpactFXData> ImpactFXDataMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    float LifeSpanTime = 3.0f;

    virtual void BeginPlay() override;

    inline AController* GetController() const;

    void MakeImpactFX(const FHitResult& HitResult);

private:
    FVector ShotDirection;

    UFUNCTION()
    virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
