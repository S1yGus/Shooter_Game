// Shooter_Game, All rights reserved.

#include "Weapons/Components/ShooterWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Components/ShooterHealthComponent.h"
#include "ShooterUtils.h"
#include "Sound/SoundCue.h"

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

    // VFX
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),                           //
                                             ImpactFXData.ImpactParticleSystem,    //
                                             FTransform(HitResult.ImpactNormal.Rotation() + FRotator(-90.0f, 0.0f, 0.0f), HitResult.ImpactPoint));

    // Sound
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactFXData.ImpactSound, HitResult.ImpactPoint);

    // Decal
    const int32 RandomDecalArrayIndex = FMath::RandHelper(ImpactFXData.ImpactDecalData.Material.Num());
    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),                                                      //
                                                                 ImpactFXData.ImpactDecalData.Material[RandomDecalArrayIndex],    //
                                                                 ImpactFXData.ImpactDecalData.Size,                               //
                                                                 HitResult.ImpactPoint,                                           //
                                                                 (HitResult.ImpactNormal * -1.0f).Rotation());

    if (DecalComponent)
    {
        DecalComponent->SetFadeOut(ImpactFXData.ImpactDecalData.LifeTime, ImpactFXData.ImpactDecalData.FadeOutTime);
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

    PlayerController->PlayerCameraManager->StartCameraShake(CameraShakeClass, Scale);
}

void UShooterWeaponFXComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(DefaultImpactFXData.ImpactDecalData.Material.Num() != 0, TEXT("DefaultImpactFXData must contains elements!"))
}