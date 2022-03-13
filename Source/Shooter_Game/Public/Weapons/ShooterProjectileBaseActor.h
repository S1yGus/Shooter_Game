// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterProjectileBaseActor.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UCameraShakeBase;

UCLASS()
class SHOOTER_GAME_API AShooterProjectileBaseActor : public AActor
{
    GENERATED_BODY()

public:
    AShooterProjectileBaseActor();

    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

    void SetDamage(float NewDamageAmount) { DamageAmount = NewDamageAmount; }

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USphereComponent* SphereComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    float LifeSpan = 3.0f;

    float DamageAmount = 0.0f;

    virtual void BeginPlay() override;

    void MakeImpactFX(const FHitResult& HitResult);
    AController* GetPawnController() const;

private:
    FVector ShotDirection;

    UFUNCTION()
    virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                 const FHitResult& Hit);
};
