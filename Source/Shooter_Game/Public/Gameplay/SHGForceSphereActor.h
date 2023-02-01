// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/SHGBaseScalableActor.h"
#include "SHGForceSphereActor.generated.h"

class USphereComponent;

UCLASS()
class SHOOTER_GAME_API ASHGForceSphereActor : public ASHGBaseScalableActor
{
    GENERATED_BODY()
public:
    ASHGForceSphereActor();

    virtual void Tick(float DeltaSeconds) override;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USphereComponent* ForceField;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float ForcePower = 4000000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", Meta = (ClampMin = "-1.0", ClampMax = "1.0"))
    float MinForcePowerMultiplier = 0.0f;

    virtual void BeginPlay() override;

private:
    TMap<AActor*, TArray<UPrimitiveComponent*>> OverlapComponentsMap;

    inline bool TryToSetConditionToInForceField(AActor* Actor, bool bCondition);

    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult);
    UFUNCTION()
    void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
