// Shooter_Game, All rights reserved.

#include "Weapons/Components/ShooterWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Components/ShooterHealthComponent.h"
#include "ShooterUtils.h"

UShooterWeaponFXComponent::UShooterWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UShooterWeaponFXComponent::MakeImactFX(const FHitResult& HitResult)
{
    FImpactFXData ImpactFXData = DefaultImpactFXData;
    if (HitResult.PhysMaterial.IsValid())
    {
        if (ImpactFXDataMap.Contains(HitResult.PhysMaterial.Get()))
        {
            ImpactFXData = ImpactFXDataMap[HitResult.PhysMaterial.Get()];
        }
    }

    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),                           //
                                             ImpactFXData.ImpactParticleSystem,    //
                                             FTransform(HitResult.ImpactNormal.Rotation() + FRotator(-90.0f, 0.0f, 0.0f), HitResult.ImpactPoint));

    int32 RandomDecalArrayIndex = FMath::RandHelper(ImpactFXData.DecalData.Material.Num());
    if (ImpactFXData.DecalData.Material.IsValidIndex(RandomDecalArrayIndex))
    {
        auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),                                                //
                                                                     ImpactFXData.DecalData.Material[RandomDecalArrayIndex],    //
                                                                     ImpactFXData.DecalData.Size,                               //
                                                                     HitResult.ImpactPoint,                                     //
                                                                     (HitResult.ImpactNormal * -1.0f).Rotation());

        if (DecalComponent)
        {
            DecalComponent->SetFadeOut(ImpactFXData.DecalData.LifeTime, ImpactFXData.DecalData.FadeOutTime);
        }
    }
}

void UShooterWeaponFXComponent::MakeTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!TraceFX)
        return;

    auto NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    NiagaraComponent->SetVariableVec3(TraceEndVariableName, TraceEnd);
}

void UShooterWeaponFXComponent::MakeCameraShake()
{
    const auto OwnerPawn = GetOwner()->GetOwner<APawn>();
    if (!OwnerPawn)
        return;

    const auto PlayerController = OwnerPawn->GetController<APlayerController>();
    if (!PlayerController || !PlayerController->PlayerCameraManager)
        return;

    if (!CameraShakeClass)
        return;

    float Scale = 1.0f;
    const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(OwnerPawn);
    if (HealthComponent)
    {
        const float CurrentHealth = HealthComponent->GetHealth();
        Scale = FMath::GetMappedRangeValueClamped(FVector2D(100.0f, 1.0f), FVector2D(0.5, 1.0), CurrentHealth);
    }

    PlayerController->PlayerCameraManager->StartMatineeCameraShake(CameraShakeClass, Scale);
}

void UShooterWeaponFXComponent::BeginPlay()
{
    Super::BeginPlay();
}