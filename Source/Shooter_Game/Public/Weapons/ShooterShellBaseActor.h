// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterShellBaseActor.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class SHOOTER_GAME_API AShooterShellBaseActor : public AActor
{
    GENERATED_BODY()

public:
    AShooterShellBaseActor();

    void SetDropDirection(const FVector& Direction) { DropDirection = Direction; }

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    float LifeSpan = 60.0f;

    virtual void BeginPlay() override;

private:
    FVector DropDirection;
};
