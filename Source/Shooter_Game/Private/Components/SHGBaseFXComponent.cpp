// Shooter_Game, All rights reserved.

#include "Components/SHGBaseFXComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Gameplay/SHGImpactIndicatorActor.h"

constexpr static int32 FootstepTraceLen = 100;
constexpr static double SpawnDecalPitch = -90.0, SpawnDecalRoll = 0.0;

USHGBaseFXComponent::USHGBaseFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USHGBaseFXComponent::MakeFootstepsFX(const FFootstepNotifyData& FootstepNotifyData)
{
    const auto OwnerCharacter = GetOwner<ACharacter>();
    if (!OwnerCharacter)
        return;

    const auto OwnerCharacterMesh = OwnerCharacter->GetMesh();
    if (!OwnerCharacterMesh)
        return;

    const auto& CurrentBoneName = FootstepNotifyData.IsLeft ? LeftFootBoneName : RightFootBoneName;
    const auto TraceStart = OwnerCharacterMesh->GetBoneLocation(CurrentBoneName);
    const auto TraceEnd = TraceStart + OwnerCharacter->GetActorUpVector() * -FootstepTraceLen;

    FHitResult HitResult;
    if (!MakeFootstepTrace(TraceStart, TraceEnd, HitResult, OwnerCharacter))
        return;

    const FFootstepsFXData* FootstepsFXData = &DefaultFootstepsFXData;
    if (HitResult.PhysMaterial.IsValid() && FootstepsFXDataMap.Contains(HitResult.PhysMaterial.Get()))
    {
        FootstepsFXData = &FootstepsFXDataMap[HitResult.PhysMaterial.Get()];
    }

    // VFX
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,                                      //
                                                   FootstepsFXData->FootstepNiagaraSystem,    //
                                                   HitResult.ImpactPoint,                     //
                                                   HitResult.ImpactNormal.Rotation());

    // Sound
    if (!FootstepNotifyData.NoSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FootstepsFXData->FootstepSound, HitResult.ImpactPoint);
    }

    // Decal
    const auto& FootstepsDecalData = FootstepNotifyData.IsLeft ? FootstepsFXData->FootstepDecalDataPair.Left : FootstepsFXData->FootstepDecalDataPair.Right;
    if (FootstepsDecalData.Material.Num() != 0)
    {
        const auto RandomDecalArrayIndex = FMath::RandHelper(FootstepsDecalData.Material.Num());
        auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(this,                                                                     //
                                                                     FootstepsDecalData.Material[RandomDecalArrayIndex],                       //
                                                                     FootstepsDecalData.Size,                                                  //
                                                                     HitResult.ImpactPoint,                                                    //
                                                                     {SpawnDecalPitch,                                                         //
                                                                      OwnerCharacterMesh->GetBoneQuaternion(CurrentBoneName).Rotator().Yaw,    //
                                                                      SpawnDecalRoll});

        if (DecalComponent)
        {
            DecalComponent->SetFadeScreenSize(FootstepsDecalData.FadeScreenSize);
            DecalComponent->SetFadeOut(FootstepsDecalData.LifeTime, FootstepsDecalData.FadeOutTime);
        }
    }
}

void USHGBaseFXComponent::SpawnImpactIndicator(float DamageAmount, const FVector& HitLocation, UPhysicalMaterial* PhysicalMaterial)
{
    if (!bShowImpactIndicators)
        return;

    const FTransform SpawnTransform{HitLocation};
    if (auto ImpactIndicator = GetWorld()->SpawnActorDeferred<ASHGImpactIndicatorActor>(ImpactIndicatorClass, SpawnTransform))
    {
        ImpactIndicator->SetDamageAmount(DamageAmount);
        if (PhysicalMaterial)
        {
            ImpactIndicator->SetImpactColor(PhysicalMaterial);
        }

        ImpactIndicator->FinishSpawning(SpawnTransform);
    }
}

void USHGBaseFXComponent::PlayDeathSound()
{
    if (const auto Owner = GetOwner())
    {
        UGameplayStatics::PlaySoundAtLocation(this, DeathSound, Owner->GetActorLocation());
    }
}

void USHGBaseFXComponent::BeginPlay()
{
    Super::BeginPlay();

    check(DeathSound);
    if (bShowImpactIndicators)
    {
        check(ImpactIndicatorClass);
    }
}

bool USHGBaseFXComponent::MakeFootstepTrace(const FVector& TraceStart, const FVector& TraceEnd, FHitResult& HitResult, const AActor* Owner)
{
    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(Owner);
    CollisionQueryParams.bReturnPhysicalMaterial = true;

    return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
}
