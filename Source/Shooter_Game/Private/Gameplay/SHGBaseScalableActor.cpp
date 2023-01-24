// Shooter_Game, All rights reserved.

#include "Gameplay/SHGBaseScalableActor.h"

constexpr static float ScalingTimerRate = 0.016f;

ASHGBaseScalableActor::ASHGBaseScalableActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASHGBaseScalableActor::BeginPlay()
{
    Super::BeginPlay();

    SetActorScale3D(FVector::ZeroVector);
    GetWorldTimerManager().SetTimer(ScaleTimerHandle, this, &ThisClass::OnChangingScale, ScalingTimerRate, true);

    if (LifeSpanTime != INFINITE_LIFESPAN)
    {
        FTimerHandle LifeSpanTimerHandle;
        GetWorldTimerManager().SetTimer(LifeSpanTimerHandle, this, &ThisClass::OnLifeSpanDone, LifeSpanTime);
    }
}

void ASHGBaseScalableActor::OnChangingScale()
{
    SetActorScale3D(FMath::VInterpTo(GetActorScale3D(), TargetScale, ScalingTimerRate, ScaleInterpSpeed));

    if (FMath::IsNearlyZero(GetActorScale3D().X)        //
        && FMath::IsNearlyZero(GetActorScale3D().Y)     //
        && FMath::IsNearlyZero(GetActorScale3D().Z))    //
    {
        OnScalingZero();
    }
    else if (FMath::IsNearlyEqual(GetActorScale3D().X, TargetScale.X)        //
             && FMath::IsNearlyEqual(GetActorScale3D().Y, TargetScale.Y)     //
             && FMath::IsNearlyEqual(GetActorScale3D().Z, TargetScale.Z))    //
    {
        OnScalingDone();
    }
}

void ASHGBaseScalableActor::OnScalingZero()
{
    Destroy();
}

void ASHGBaseScalableActor::OnScalingDone()
{
    GetWorldTimerManager().ClearTimer(ScaleTimerHandle);
}

void ASHGBaseScalableActor::OnLifeSpanDone()
{
    TargetScale = FVector::ZeroVector;
    GetWorldTimerManager().SetTimer(ScaleTimerHandle, this, &ThisClass::OnChangingScale, ScalingTimerRate, true);
}
