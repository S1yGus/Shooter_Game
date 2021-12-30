// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterPickupBaseActor.generated.h"

class USphereComponent;

UCLASS()
class SHOOTER_GAME_API AShooterPickupBaseActor : public AActor
{
	GENERATED_BODY()
	
public:
	AShooterPickupBaseActor();

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USphereComponent* SphereComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool SingleUsePickup = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (EditCondition = "!SingleUsePickup"))
    float RespawnTime = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float RotationYaw = 1.0f;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
    TArray<APawn*> OverlapedPawns;
    FTimerHandle RotationTimerHandle;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
    virtual bool GivePickupTo(APawn* PlayerPawn);
    void TryToGivePickupToOverlapedPawns();
    void PickupWasTaken();
    void Respawn();
    void RotatePickup();
};
