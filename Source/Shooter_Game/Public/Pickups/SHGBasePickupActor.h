// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHGBasePickupActor.generated.h"

class USphereComponent;
class USoundCue;

UCLASS()
class SHOOTER_GAME_API ASHGBasePickupActor : public AActor
{
    GENERATED_BODY()

public:
    ASHGBasePickupActor();

    virtual void Tick(float DeltaSeconds) override;

    bool CanBeTaken() const;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USphereComponent* SphereComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
    bool bRespawnable = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn", Meta = (EditCondition = "bRespawnable", Units = "s"))
    float RespawnTime = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", Meta = (Units = "deg"))
    float RotationYaw = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundCue* TakenPickupSound;

    virtual void BeginPlay() override;

private:
    TSet<AActor*> OverlapActors;
    FTimerHandle RespawnTimerHandle;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
    virtual bool TryToGivePickupTo(AActor* Actor);

    void TryToGivePickupToOverlapActors();
    void PickupWasTaken();

    void OnRespawn();
    void OnRotatePickup();
};
