#include "SHGUtils.h"
#include "Player/ShooterPlayerState.h"

bool SHGUtils::AreEnemies(AController* Controller1, AController* Controller2)
{
    if (!Controller1 || !Controller2 || Controller1 == Controller2)
        return false;

    const auto ShooterPlayerState1 = Cast<AShooterPlayerState>(Controller1->PlayerState);
    const auto ShooterPlayerState2 = Cast<AShooterPlayerState>(Controller2->PlayerState);

    return ShooterPlayerState1 && ShooterPlayerState2 && ShooterPlayerState1->GetTeamID() != ShooterPlayerState2->GetTeamID();
}

static bool ComponentIsDamageableFrom(UPrimitiveComponent* VictimComp,        //
                                      const FVector& Origin,                  //
                                      const AActor* IgnoredActor,             //
                                      const TArray<AActor*>& IgnoreActors,    //
                                      ECollisionChannel TraceChannel,         //
                                      FHitResult& OutHitResult)
{
    FCollisionQueryParams LineParams{FName{}, true, IgnoredActor};
    LineParams.AddIgnoredActors(IgnoreActors);

    // Do a trace from origin to middle of box.
    UWorld* const World = VictimComp->GetWorld();
    check(World);

    const FVector TraceEnd = VictimComp->Bounds.Origin;
    FVector TraceStart = Origin;
    if (Origin == TraceEnd)
    {
        // Tiny nudge so LineTraceSingle doesn't early out with no hits.
        TraceStart.Z += 0.01f;
    }

    check(TraceChannel != ECollisionChannel::ECC_MAX);
    if (World->LineTraceSingleByChannel(OutHitResult, TraceStart, TraceEnd, TraceChannel, LineParams))
    {
        if (OutHitResult.Component == VictimComp)
        {
            // If blocking hit was the victim component, it is visible.
            return true;
        }
        else
        {
            // If we hit something else blocking, it's not.
            return false;
        }
    }

    // Didn't hit anything, assume nothing blocking the damage and victim is consequently visible,
    // but since we don't have a hit result to pass back, construct a simple one, modeling the damage as having hit a point at the component's center.
    const FVector FakeHitLocation = VictimComp->GetComponentLocation();
    const FVector FakeHitNormal = (Origin - FakeHitLocation).GetSafeNormal();    // Normal points back toward the epicenter.
    OutHitResult = FHitResult(VictimComp->GetOwner(), VictimComp, FakeHitLocation, FakeHitNormal);

    return true;
}

bool SHGUtils::ApplyRadialDamageWithImpulse(const UObject* WorldContextObject,           //
                                            float BaseDamage,                            //
                                            float MinimumDamage,                         //
                                            float ImpulsePower,                          //
                                            const FVector& Origin,                       //
                                            float DamageInnerRadius,                     //
                                            float DamageOuterRadius,                     //
                                            bool bDoFullDamage,                          //
                                            TSubclassOf<UDamageType> DamageTypeClass,    //
                                            const TArray<AActor*>& IgnoreActors,         //
                                            AActor* DamageCauser,                        //
                                            AController* InstigatedByController,         //
                                            ECollisionChannel DamagePreventionChannel)
{
    FCollisionQueryParams SphereParams{FName{}, false, DamageCauser};
    SphereParams.AddIgnoredActors(IgnoreActors);

    // Query scene to see what we hit.
    TArray<FOverlapResult> Overlaps;
    if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams{FCollisionObjectQueryParams::InitType::AllDynamicObjects},
                                        FCollisionShape::MakeSphere(DamageOuterRadius), SphereParams);
    }

    // Collate into per-actor list of hit components.
    TMap<AActor*, TArray<FHitResult>> OverlapComponentMap;
    for (const auto& Overlap : Overlaps)
    {
        const auto OverlapActor = Overlap.OverlapObjectHandle.FetchActor();
        if (OverlapActor && OverlapActor->CanBeDamaged() && OverlapActor != DamageCauser && Overlap.Component.IsValid())
        {
            FHitResult Hit;
            if (ComponentIsDamageableFrom(Overlap.Component.Get(), Origin, DamageCauser, IgnoreActors, DamagePreventionChannel, Hit))
            {
                TArray<FHitResult>& HitList = OverlapComponentMap.FindOrAdd(OverlapActor);
                HitList.Add(Hit);
            }
        }
    }

    bool bAppliedDamage = false;
    if (!OverlapComponentMap.IsEmpty())
    {
        // Make sure we have a good damage type.
        const auto ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());

        FRadialDamageEvent DmgEvent;
        DmgEvent.DamageTypeClass = ValidDamageTypeClass;
        DmgEvent.Origin = Origin;
        DmgEvent.Params = FRadialDamageParams(BaseDamage, MinimumDamage, DamageInnerRadius, DamageOuterRadius, bDoFullDamage ? 0.0f : 1.0f);

        // Call damage and impulse function on each affected actors.
        for (const auto& [Victim, ComponentHits] : OverlapComponentMap)
        {
            DmgEvent.ComponentHits = ComponentHits;

            Victim->TakeDamage(BaseDamage, DmgEvent, InstigatedByController, DamageCauser);

            for (const auto& Hit : ComponentHits)
            {
                if (!Hit.Component.IsValid())
                    continue;

                const auto Component = Hit.Component.Get();
                if (Component->IsSimulatingPhysics())
                {
                    Component->AddRadialImpulse(Origin, DamageOuterRadius, ImpulsePower, ERadialImpulseFalloff::RIF_Linear);
                }
            }

            bAppliedDamage = true;
        }
    }

    return bAppliedDamage;
}

FString SHGUtils::FormatStringFromInt(int32 InValue, int32 MinLen, TCHAR FillChar)
{
    FString ResultStr = FString::FromInt(InValue);
    const auto ToAdd = MinLen - ResultStr.Len();
    if (ToAdd > 0)
    {
        ResultStr = FString::ChrN(ToAdd, FillChar).Append(ResultStr);
    }

    return ResultStr;
}
