// Shooter_Game, All rights reserved.

#include "Weapons/Components/SHGWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

USHGWeaponFXComponent::USHGWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USHGWeaponFXComponent::BeginPlay()
{
    Super::BeginPlay();

    check(NoAmmoSound);
    check(SwitchModeSound);
    check(SwitchFlashlight);
}

void USHGWeaponFXComponent::MakeFireFX(USceneComponent* SceneComponent, const FName& SocketName, bool bAlternative)
{
    const FWeaponFXData* WeaponFXData = bAlternative ? &AlternativeWeaponFXData : &MainWeaponFXData;
    UNiagaraFunctionLibrary::SpawnSystemAttached(WeaponFXData->MuzzleFX,           //
                                                 SceneComponent,                   //
                                                 SocketName,                       //
                                                 FVector::Zero(),                  //
                                                 FRotator::ZeroRotator,            //
                                                 EAttachLocation::SnapToTarget,    //
                                                 true);

    UGameplayStatics::SpawnSoundAttached(WeaponFXData->FireSound, SceneComponent, SocketName);
    if (const auto ComponentOwner = GetOwner())
    {
        ComponentOwner->MakeNoise(WeaponFXData->FireAILoudness, ComponentOwner->GetOwner<APawn>(), SceneComponent->GetSocketLocation(SocketName));
    }

    MakeCameraShake(WeaponFXData->FireCameraShakeClass);
}

void USHGWeaponFXComponent::PlayNoAmmoSound(USceneComponent* SceneComponent, const FName& SocketName)
{
    UGameplayStatics::SpawnSoundAttached(NoAmmoSound, SceneComponent, SocketName);
}

void USHGWeaponFXComponent::PlaySwitchModeSound(USceneComponent* SceneComponent, const FName& SocketName)
{
    UGameplayStatics::SpawnSoundAttached(SwitchModeSound, SceneComponent, SocketName);
}

void USHGWeaponFXComponent::PlaySwitchFlashlightSound(USceneComponent* SceneComponent, const FName& SocketName)
{
    UGameplayStatics::SpawnSoundAttached(SwitchFlashlight, SceneComponent, SocketName);
}

void USHGWeaponFXComponent::MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale)
{
    const auto Owner = GetOwner();
    if (!Owner)
        return;

    const auto OwnerPawn = Owner->GetOwner<APawn>();
    if (!OwnerPawn)
        return;

    const auto PC = OwnerPawn->GetController<APlayerController>();
    if (!PC || !PC->PlayerCameraManager)
        return;

    PC->PlayerCameraManager->StartCameraShake(CameraShakeClass, Scale);
}
