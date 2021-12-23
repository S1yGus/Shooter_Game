// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterProjectileBaseActor.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOOTER_GAME_API AShooterProjectileBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterProjectileBaseActor();

	void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    float LifeSpan = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile", Meta = (ClampMin = "0"))
    float DamageAmount = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile", Meta = (ClampMin = "0"))
    float DamageRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    bool DoFullDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<UMatineeCameraShake> CameraShakeClass;

	virtual void BeginPlay() override;

private:
    FVector ShotDirection;

	UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent * HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          FVector NormalImpulse, const FHitResult& Hit);

	AController* GetController() const;
    void MakeCameraShake();
};
