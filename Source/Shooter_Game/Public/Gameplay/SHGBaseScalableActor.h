// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHGBaseScalableActor.generated.h"

#define INFINITE_LIFESPAN -1.0f

UCLASS()
class SHOOTER_GAME_API ASHGBaseScalableActor : public AActor
{
    GENERATED_BODY()

public:
    ASHGBaseScalableActor();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", Meta = (ClampMin = "-1.0"))
    float LifeSpanTime = INFINITE_LIFESPAN;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    FVector TargetScale{1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    float ScaleInterpSpeed = 2.0f;

    virtual void BeginPlay() override;

private:
    FTimerHandle ScaleTimerHandle;

    void OnChangingScale();
    virtual void OnScalingZero();
    virtual void OnScalingDone();
    virtual void OnLifeSpanDone();
};
