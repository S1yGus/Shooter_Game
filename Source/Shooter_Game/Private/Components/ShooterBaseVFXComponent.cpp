// Shooter_Game, All rights reserved.

#include "Components/ShooterBaseVFXComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Gameplay/SHGImpactIndicatorActor.h"
#include "Components/SHGHealthComponent.h"

USHGBaseVFXComponent::USHGBaseVFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USHGBaseVFXComponent::MakeFootstepVFX(const FFootstepNotifyData& FootstepNotifyData)
{
    const auto OwnerCharacter = GetOwner<ACharacter>();
    if (!OwnerCharacter)
        return;

    const auto CurrentBoneName = FootstepNotifyData.IsLeft ? LeftFootBoneName : RightFootBoneName;
    const FVector TraceStart = OwnerCharacter->GetMesh()->GetBoneLocation(CurrentBoneName);
    const FVector TraceEnd = TraceStart + OwnerCharacter->GetActorUpVector() * -100;

    FHitResult HitResult;

    if (!MakeFootstepTrace(TraceStart, TraceEnd, HitResult))
        return;

    auto FootstepFXData = DefaultFootstepFXData;

    if (HitResult.PhysMaterial.IsValid())
    {
        if (FootstepsFXDataMap.Contains(FootstepNotifyData.LocomotionType))
        {
            const auto TempFootstepFXDataMap = FootstepsFXDataMap[FootstepNotifyData.LocomotionType];
            if (TempFootstepFXDataMap.FootstepFXDataMap.Contains(HitResult.PhysMaterial.Get()))
            {
                FootstepFXData = TempFootstepFXDataMap.FootstepFXDataMap[HitResult.PhysMaterial.Get()];
            }
        }
    }

    // VFX
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),                              //
                                                   FootstepFXData.FootstepNiagaraSystem,    //
                                                   HitResult.ImpactPoint,                   //
                                                   HitResult.ImpactNormal.Rotation() + FRotator(-90.0f, 0.0f, 0.0f));

    // Sound
    if (!FootstepNotifyData.NoSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepFXData.FootstepSound, HitResult.ImpactPoint);
    }

    // Decal
    const auto FootstepDecalData = FootstepNotifyData.IsLeft ? FootstepFXData.FootstepDecalDataPair.Left : FootstepFXData.FootstepDecalDataPair.Right;
    if (FootstepDecalData.Material.Num() == 0)
        return;

    const int32 RandomDecalArrayIndex = FMath::RandHelper(FootstepDecalData.Material.Num());
    const auto SpawnDecalRotator = FRotator(-90, OwnerCharacter->GetMesh()->GetBoneQuaternion(CurrentBoneName).Rotator().Yaw, 0);
    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),                                           //
                                                                 FootstepDecalData.Material[RandomDecalArrayIndex],    //
                                                                 FootstepDecalData.Size,                               //
                                                                 HitResult.ImpactPoint,                                //
                                                                 SpawnDecalRotator);

    if (!DecalComponent)
        return;

    DecalComponent->SetFadeOut(FootstepDecalData.LifeTime, FootstepDecalData.FadeOutTime);
}

void USHGBaseVFXComponent::SpawnImpactIndicator(float DamageAmount, const FVector& HitLocation, UPhysicalMaterial* PhysicalMaterial)
{
    if (!ShowImpactIndicators || !ImpactIndicatorClass)
        return;

    if (!GetOwner()->FindComponentByClass<USHGHealthComponent>() || GetOwner()->FindComponentByClass<USHGHealthComponent>()->IsDead())
        return;

    FTransform SpawnTransform = FTransform(HitLocation);
    const auto ImpactIndicator = GetWorld()->SpawnActorDeferred<ASHGImpactIndicatorActor>(ImpactIndicatorClass, SpawnTransform);
    if (!ImpactIndicator)
        return;

    ImpactIndicator->SetDamageAmount(DamageAmount);
    if (PhysicalMaterial)
    {
        ImpactIndicator->SetImpactColor(PhysicalMaterial);
    }

    ImpactIndicator->FinishSpawning(SpawnTransform);
}

void USHGBaseVFXComponent::PlayDeathSound()
{
    if (!GetOwner())
        return;

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetOwner()->GetActorLocation());
}

void USHGBaseVFXComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool USHGBaseVFXComponent::MakeFootstepTrace(const FVector& TraceStart, const FVector& TraceEnd, FHitResult& HitResult)
{
    const auto OwnerCharacter = GetOwner<ACharacter>();
    if (!OwnerCharacter)
        return false;

    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(OwnerCharacter);
    CollisionQueryParams.bReturnPhysicalMaterial = true;

    return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
}
