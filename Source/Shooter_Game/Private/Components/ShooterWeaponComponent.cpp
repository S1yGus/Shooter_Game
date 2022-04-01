// Shooter_Game, All rights reserved.

#include "Components/ShooterWeaponComponent.h"
#include "Weapons/ShooterBaseWeaponActor.h"
#include "Animations/ShooterEquipFinishedAnimNotify.h"
#include "Animations/ShooterReloadFinishedAnimNotify.h"
#include "Player/ShooterBaseCharacter.h"
#include "Animations/AnimUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

constexpr static float ZoomTimerRate = 0.007f;
constexpr static float ZoomChangeSpeed = 12.0f;

UShooterWeaponComponent::UShooterWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UShooterWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    CurrentWeapon = nullptr;

    for (auto Weapon : GetWeaponsMapValueArray())
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }

    WeaponsMap.Empty();
}

void UShooterWeaponComponent::StartFire()
{
    if (!CanFire())
        return;

    CurrentWeapon->StartFire();
}

void UShooterWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
        return;

    CurrentWeapon->StopFire();
}

void UShooterWeaponComponent::Zoom(bool Condition)
{
    if (!CurrentWeapon || !CurrentWeapon->IsCanZoom())
        return;

    if (!GetPlayerController() || !GetPlayerController()->PlayerCameraManager)
        return;

    if (!GetOwner<AShooterBaseCharacter>() || GetOwner<AShooterBaseCharacter>()->IsSprinting() && Condition)
        return;

    if (!DefaultFOVAngle)
        if (!GetCurrentFOV(DefaultFOVAngle))
            return;

    TargetFOVAngle = Condition ? CurrentWeapon->GetZoomFOVAngle() : DefaultFOVAngle;
    ZoomingNow = Condition;

    if (GetWorld()->GetTimerManager().IsTimerActive(ZoomTimerHandle))
        return;

    GetWorld()->GetTimerManager().SetTimer(ZoomTimerHandle, this, &UShooterWeaponComponent::ZoomTick, ZoomTimerRate, true);
}

void UShooterWeaponComponent::SwitchFireMode()
{
    if (!CurrentWeapon)
        return;

    CurrentWeapon->SwitchFireMode();
}

void UShooterWeaponComponent::SwitchFlashlight()
{
    FlashlightOff = !FlashlightOff;

    if (!CurrentWeapon)
        return;

    CurrentWeapon->TurnOffFlashlight(FlashlightOff);
}

void UShooterWeaponComponent::NextWeapon()
{
    if (!CanEquip() || WeaponsMap.Num() == 0)
        return;

    CurrentWeaponIndex = ++CurrentWeaponIndex % StaticCast<int32>(EWeaponType::Max);
    if (WeaponsMap.Contains(StaticCast<EWeaponType>(CurrentWeaponIndex)))
    {
        EquipWeapon(StaticCast<EWeaponType>(CurrentWeaponIndex));
        return;
    }

    NextWeapon();
}

void UShooterWeaponComponent::PreviousWeapon()
{
    if (!CanEquip() || WeaponsMap.Num() == 0)
        return;

    --CurrentWeaponIndex;
    if (CurrentWeaponIndex == -1)
    {
        CurrentWeaponIndex = StaticCast<int32>(EWeaponType::Max) - 1;
    }
    CurrentWeaponIndex %= StaticCast<int32>(EWeaponType::Max);
    if (WeaponsMap.Contains(StaticCast<EWeaponType>(CurrentWeaponIndex)))
    {
        EquipWeapon(StaticCast<EWeaponType>(CurrentWeaponIndex));
        return;
    }

    PreviousWeapon();
}

void UShooterWeaponComponent::EquipWeapon(EWeaponType WeapontType)
{
    if (!WeaponsMap.Contains(WeapontType) || !CanEquip() || !GetOwner<ACharacter>())
        return;

    if (CurrentWeapon)
    {
        if (CurrentWeapon->GetWeaponType() == WeapontType)
            return;

        StopFire();
        Zoom(false);
        CurrentWeapon->TurnOffFlashlight(true);

        AttachToSocket(CurrentWeapon, GetOwner<ACharacter>()->GetMesh(), CurrentWeapon->GetArmorySocketName());
    }

    CurrentWeapon = WeaponsMap[WeapontType];
    AttachToSocket(CurrentWeapon, GetOwner<ACharacter>()->GetMesh(), WeaponEquipSocketName);

    CurrentWeapon->TurnOffFlashlight(FlashlightOff);

    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });
    CurrentReloadAnimMontage = CurrentWeaponData->ReloadAnimMontage;

    EquipMontageInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void UShooterWeaponComponent::ReloadWeapon()
{
    ChangeClip();
}

bool UShooterWeaponComponent::TryToAddAmmo(TSubclassOf<AShooterBaseWeaponActor> WeaponClass, int32 ClipsAmount)
{
    for (const auto Weapon : GetWeaponsMapValueArray())
    {
        if (Weapon->IsA(WeaponClass))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }

    return false;
}

bool UShooterWeaponComponent::TryToAddWeapon(const FWeaponData& NewWeaponData)
{
    for (const auto Weapon : GetWeaponsMapValueArray())
    {
        if (Weapon->GetClass() == NewWeaponData.WeaponClass)
        {
            if (Weapon->IsNumberOfClipsMax())
            {
                return false;
            }
            else
            {
                Weapon->RestoreMaxNumberOfClips();
                return true;
            }
        }
    }

    const auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UShooterReloadFinishedAnimNotify>(NewWeaponData.ReloadAnimMontage);
    if (!ReloadFinishedNotify)
        return false;

    ReloadFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnReloadFinished);

    WeaponData.Add(NewWeaponData);

    if (!SpawnWeapon(NewWeaponData.WeaponClass))
    {
        WeaponData.RemoveAllSwap([&](const FWeaponData& WeaponData) { return WeaponData.WeaponClass == NewWeaponData.WeaponClass; });
        return false;
    }

    return true;
}

bool UShooterWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& Data) const
{
    if (!CurrentWeapon)
        return false;

    Data = CurrentWeapon->GetUIData();
    return true;
}

bool UShooterWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& Data) const
{
    if (!CurrentWeapon)
        return false;

    Data = CurrentWeapon->GetAmmoData();
    return true;
}

void UShooterWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    check(EquipAnimMontage);

    BindNotifys();

    SpawnWeapons();
    NextWeapon();
}

APlayerController* UShooterWeaponComponent::GetPlayerController() const
{
    return GetOwner<APawn>() ? GetOwner<APawn>()->GetController<APlayerController>() : nullptr;
}

TArray<AShooterBaseWeaponActor*> UShooterWeaponComponent::GetWeaponsMapValueArray() const
{
    TArray<AShooterBaseWeaponActor*> WeaponsArray;
    WeaponsMap.GenerateValueArray(WeaponsArray);
    return WeaponsArray;
}

void UShooterWeaponComponent::SpawnWeapons()
{
    for (const auto OneWeaponData : WeaponData)
    {
        SpawnWeapon(OneWeaponData.WeaponClass);
    }
}

bool UShooterWeaponComponent::SpawnWeapon(TSubclassOf<AShooterBaseWeaponActor> WeaponClass)
{
    if (!GetOwner<ACharacter>())
        return false;

    const auto Weapon = GetWorld()->SpawnActor<AShooterBaseWeaponActor>(WeaponClass);
    if (!Weapon)
        return false;

    Weapon->OnClipEmpty.AddUObject(this, &UShooterWeaponComponent::OnClipEmpty);
    Weapon->SetOwner(GetOwner());

    if (WeaponsMap.Contains(Weapon->GetWeaponType()))
    {
        if (CurrentWeapon->GetWeaponType() == Weapon->GetWeaponType())
        {
            CurrentWeapon = nullptr;
            EquipMontageInProgress = false;
            ReloadMontageInProgress = false;
        }

        WeaponsMap[Weapon->GetWeaponType()]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        WeaponsMap[Weapon->GetWeaponType()]->Destroy();
    }

    WeaponsMap.Add(Weapon->GetWeaponType(), Weapon);
    AttachToSocket(Weapon, GetOwner<ACharacter>()->GetMesh(), Weapon->GetArmorySocketName());

    if (!CurrentWeapon)
    {
        EquipWeapon(Weapon->GetWeaponType());
    }

    return true;
}

void UShooterWeaponComponent::AttachToSocket(AShooterBaseWeaponActor* Weapon, USceneComponent* ScenComponent, FName SocketName)
{
    if (!Weapon || !ScenComponent)
        return;

    FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(ScenComponent, AttachmentRule, SocketName);
}

void UShooterWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage)
{
    if (!GetOwner<ACharacter>())
        return;

    GetOwner<ACharacter>()->PlayAnimMontage(AnimMontage);
}

void UShooterWeaponComponent::BindNotifys()
{
    const auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UShooterEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnEquipFinished);
    }

    for (const auto OneWeaponData : WeaponData)
    {
        const auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UShooterReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishedNotify)
            continue;

        ReloadFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnReloadFinished);
    }
}

void UShooterWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    if (!GetOwner<ACharacter>() && GetOwner<ACharacter>()->GetMesh() == MeshComp)
        return;

    EquipMontageInProgress = false;
}

void UShooterWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
    if (!GetOwner<ACharacter>() || GetOwner<ACharacter>()->GetMesh() != MeshComp)
        return;

    ReloadMontageInProgress = false;
}

bool UShooterWeaponComponent::CanFire() const
{
    if (!GetOwner<AShooterBaseCharacter>())
        return false;

    return CurrentWeapon && !EquipMontageInProgress && !ReloadMontageInProgress && !GetOwner<AShooterBaseCharacter>()->IsSprinting();
}

bool UShooterWeaponComponent::CanEquip() const
{
    return !EquipMontageInProgress && !ReloadMontageInProgress;
}

bool UShooterWeaponComponent::CanReload() const
{
    return CurrentWeapon && !EquipMontageInProgress && !ReloadMontageInProgress;
}

void UShooterWeaponComponent::OnClipEmpty()
{
    ChangeClip();
}

void UShooterWeaponComponent::ChangeClip()
{
    if (!CanReload() || !CurrentWeapon->ReloadClip())
        return;

    CurrentWeapon->StopFire();

    ReloadMontageInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}

bool UShooterWeaponComponent::GetCurrentFOV(float& CurrentFOV)
{
    if (!GetPlayerController() || !GetPlayerController()->PlayerCameraManager)
        return false;

    CurrentFOV = GetPlayerController()->PlayerCameraManager->GetFOVAngle();
    return true;
}

void UShooterWeaponComponent::ZoomTick()
{
    if (!GetPlayerController() || !GetPlayerController()->PlayerCameraManager)
        return;

    const auto NewFOVAngle = FMath::FInterpTo(GetPlayerController()->PlayerCameraManager->GetFOVAngle(), TargetFOVAngle, ZoomTimerRate, ZoomChangeSpeed);
    GetPlayerController()->PlayerCameraManager->SetFOV(NewFOVAngle);

    if (FMath::IsNearlyEqual(NewFOVAngle, TargetFOVAngle))
    {
        GetWorld()->GetTimerManager().ClearTimer(ZoomTimerHandle);
    }
}