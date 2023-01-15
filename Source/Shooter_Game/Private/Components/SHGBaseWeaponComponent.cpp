// Shooter_Game, All rights reserved.

#include "Components/SHGBaseWeaponComponent.h"
#include "Weapons/SHGBaseWeaponActor.h"
#include "Animations/ShooterEquipFinishedAnimNotify.h"
#include "Animations/ShooterReloadFinishedAnimNotify.h"
#include "Player/ShooterBaseCharacter.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

constexpr static float ZoomTimerRate = 0.015f;
constexpr static float ZoomChangeSpeed = 15.0f;

USHGBaseWeaponComponent::USHGBaseWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USHGBaseWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    check(EquipAnimMontage);
    check(WeaponEquipSocketName != NAME_None);

    SpawnWeapons();
    NextWeapon();
    BindNotifys();
}

void USHGBaseWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
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

void USHGBaseWeaponComponent::StartFire()
{
    if (!CanFire())
        return;

    CurrentWeapon->StartFire();
}

void USHGBaseWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
        return;

    CurrentWeapon->StopFire();
}

void USHGBaseWeaponComponent::SwitchFireMode()
{
    if (!CurrentWeapon)
        return;

    CurrentWeapon->SwitchFireMode();
}

void USHGBaseWeaponComponent::Zoom(bool bCondition, bool bForce)
{
    if (!CurrentWeapon || !CurrentWeapon->CanZoom())
        return;

    const auto PC = GetPlayerController();
    if (!PC || !PC->PlayerCameraManager)
        return;

    const auto OwnerCharacter = GetOwner<AShooterBaseCharacter>();
    if (!OwnerCharacter || (OwnerCharacter->IsSprinting() && bCondition))
        return;

    if (!DefaultFOVAngle)
        if (!GetCurrentFOV(DefaultFOVAngle))
            return;

    TargetFOVAngle = bCondition ? CurrentWeapon->GetZoomFOVAngle() : DefaultFOVAngle;

    if (bForce)
    {
        GetWorld()->GetTimerManager().ClearTimer(ZoomTimerHandle);
        PC->PlayerCameraManager->SetFOV(TargetFOVAngle);
    }
    else
    {
        GetWorld()->GetTimerManager().SetTimer(ZoomTimerHandle, this, &ThisClass::ZoomUpdate, ZoomTimerRate, true);
    }

    OnZoom.Broadcast(bCondition);
}

void USHGBaseWeaponComponent::SwitchFlashlight()
{
    if (!CurrentWeapon)
        return;

    bFlashlightCondition = !bFlashlightCondition;
    SetFlashlight(bFlashlightCondition);
}

void USHGBaseWeaponComponent::NextWeapon()
{
    if (!CanEquip() || WeaponsMap.IsEmpty())
        return;

    CurrentWeaponIndex = ++CurrentWeaponIndex % StaticCast<int32>(EWeaponType::Max);
    if (!TryToEquipWeapon(StaticCast<EWeaponType>(CurrentWeaponIndex)) && WeaponsMap.Num() != 1)    // Do not run recursion if only one weapon is available.
    {
        NextWeapon();
    }
}

void USHGBaseWeaponComponent::PreviousWeapon()
{
    if (!CanEquip() || WeaponsMap.IsEmpty())
        return;

    --CurrentWeaponIndex;
    if (CurrentWeaponIndex == -1)
    {
        CurrentWeaponIndex = StaticCast<int32>(EWeaponType::Max) - 1;
    }

    if (!TryToEquipWeapon(StaticCast<EWeaponType>(CurrentWeaponIndex)) && WeaponsMap.Num() != 1)    // Do not run recursion if only one weapon is available.
    {
        PreviousWeapon();
    }
}

void USHGBaseWeaponComponent::EquipWeapon(EWeaponType WeapontType)
{
    if (!CanEquip())
        return;

    TryToEquipWeapon(WeapontType);
}

void USHGBaseWeaponComponent::ReloadWeapon()
{
    if (!CanReload())
        return;

    ChangeClip();
}

bool USHGBaseWeaponComponent::TryToAddAmmo(TSubclassOf<ASHGBaseWeaponActor> WeaponClass, int32 ClipsAmount)
{
    for (const auto Weapon : GetWeaponsMapValueArray())
    {
        if (Weapon->IsA(WeaponClass))
        {
            const bool bCondition = Weapon->TryToAddAmmo(ClipsAmount);
            OnAmmoInfoChanged.Broadcast(CurrentWeapon->GetAmmoData());
            return bCondition;
        }
    }

    return false;
}

bool USHGBaseWeaponComponent::TryToAddWeapon(TSubclassOf<ASHGBaseWeaponActor> WeaponClass)
{
    for (const auto Weapon : GetWeaponsMapValueArray())
    {
        if (Weapon->IsA(WeaponClass))
        {
            return TryToAddAmmo(WeaponClass, Weapon->GetMaxNumberOfClips() + 1);    // + 1 in case of empty clip.
        }
    }

    if (SpawnWeapon(WeaponClass))
    {
        return true;
    }

    return false;
}

bool USHGBaseWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& Data) const
{
    if (!CurrentWeapon)
        return false;

    Data = CurrentWeapon->GetUIData();

    return true;
}

bool USHGBaseWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& Data) const
{
    if (!CurrentWeapon)
        return false;

    Data = CurrentWeapon->GetAmmoData();

    return true;
}

bool USHGBaseWeaponComponent::CanFire() const
{
    if (const auto OwnerCharacter = GetOwner<AShooterBaseCharacter>())
    {
        return CurrentWeapon && !bEquipMontageInProgress && !bReloadMontageInProgress && !OwnerCharacter->IsSprinting();
    }

    return false;
}

bool USHGBaseWeaponComponent::CanEquip() const
{
    return !bEquipMontageInProgress && !bReloadMontageInProgress;
}

bool USHGBaseWeaponComponent::TryToEquipWeapon(EWeaponType WeaponType)
{
    const auto OwnerCharacter = GetOwner<ACharacter>();
    if (!WeaponsMap.Contains(WeaponType) || !OwnerCharacter)
        return false;

    if (CurrentWeapon)
    {
        if (CurrentWeapon->GetWeaponData().WeaponType == WeaponType)
            return false;

        StopFire();
        Zoom(false);
        SetFlashlight(false);

        AttachToSocket(CurrentWeapon, OwnerCharacter->GetMesh(), CurrentWeapon->GetWeaponData().WeaponArmorySocketName);
    }

    CurrentWeapon = WeaponsMap[WeaponType];
    AttachToSocket(CurrentWeapon, OwnerCharacter->GetMesh(), WeaponEquipSocketName);
    CurrentReloadAnimMontage = CurrentWeapon->GetWeaponData().ReloadAnimMontage;
    SetFlashlight(bFlashlightCondition);

    bEquipMontageInProgress = true;
    PlayAnimMontage(EquipAnimMontage);

    OnWeaponChanged.Broadcast(CurrentWeapon->GetUIData(), CurrentWeapon->GetAmmoData());

    return true;
}

bool USHGBaseWeaponComponent::GetCurrentFOV(float& CurrentFOV)
{
    const auto PC = GetPlayerController();
    if (!PC || !PC->PlayerCameraManager)
        return false;

    CurrentFOV = PC->PlayerCameraManager->GetFOVAngle();
    return true;
}

void USHGBaseWeaponComponent::ZoomUpdate()
{
    const auto PC = GetPlayerController();
    if (!PC || !PC->PlayerCameraManager)
        return;

    const auto NewFOVAngle = FMath::FInterpTo(PC->PlayerCameraManager->GetFOVAngle(), TargetFOVAngle, ZoomTimerRate, ZoomChangeSpeed);
    PC->PlayerCameraManager->SetFOV(NewFOVAngle);

    if (FMath::IsNearlyEqual(NewFOVAngle, TargetFOVAngle))
    {
        GetWorld()->GetTimerManager().ClearTimer(ZoomTimerHandle);
    }
}

void USHGBaseWeaponComponent::SetFlashlight(bool bCondition)
{
    CurrentWeapon->SetFlashlight(bCondition);
}

void USHGBaseWeaponComponent::ChangeClip()
{
    if (!CurrentWeapon->ReloadClip())
        return;

    CurrentWeapon->StopFire();

    bReloadMontageInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);

    OnAmmoInfoChanged.Broadcast(CurrentWeapon->GetAmmoData());
}

APlayerController* USHGBaseWeaponComponent::GetPlayerController() const
{
    const auto OwnerPawn = GetOwner<APawn>();
    return OwnerPawn ? OwnerPawn->GetController<APlayerController>() : nullptr;
}

TArray<ASHGBaseWeaponActor*> USHGBaseWeaponComponent::GetWeaponsMapValueArray() const
{
    TArray<ASHGBaseWeaponActor*> WeaponsArray;
    WeaponsMap.GenerateValueArray(WeaponsArray);
    return WeaponsArray;
}

bool USHGBaseWeaponComponent::CanReload() const
{
    return CurrentWeapon && !bEquipMontageInProgress && !bReloadMontageInProgress;
}

void USHGBaseWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage)
{
    if (const auto OwnerCharacter = GetOwner<ACharacter>())
    {
        OwnerCharacter->PlayAnimMontage(AnimMontage);
    }
}

void USHGBaseWeaponComponent::AttachToSocket(ASHGBaseWeaponActor* Weapon, USceneComponent* ScenComponent, FName SocketName)
{
    if (!Weapon || !ScenComponent)
        return;

    FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(ScenComponent, AttachmentRule, SocketName);
}

void USHGBaseWeaponComponent::SpawnWeapons()
{
    for (const auto OneWeapon : Weapons)
    {
        SpawnWeapon(OneWeapon);
    }
}

bool USHGBaseWeaponComponent::SpawnWeapon(TSubclassOf<ASHGBaseWeaponActor> WeaponClass)
{
    const auto OwnerCharacter = GetOwner<ACharacter>();
    if (!OwnerCharacter)
        return false;

    const auto Weapon = GetWorld()->SpawnActor<ASHGBaseWeaponActor>(WeaponClass);
    if (!Weapon)
        return false;

    const auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UShooterReloadFinishedAnimNotify>(Weapon->GetWeaponData().ReloadAnimMontage);
    if (!ReloadFinishedNotify)
    {
        UE_LOG(WeaponComponentLog, Error, TEXT("ReloadFinishedNotify wasn't found for: %s!"), *Weapon->GetName());
        checkNoEntry();

        Weapon->Destroy();
        return false;
    }

    ReloadFinishedNotify->OnNotified.AddUObject(this, &ThisClass::OnReloadFinished);
    Weapon->OnClipEmpty.AddUObject(this, &ThisClass::OnClipEmpty);
    Weapon->OnFired.AddUObject(this, &ThisClass::OnFired);
    Weapon->SetOwner(GetOwner());

    if (WeaponsMap.Contains(Weapon->GetWeaponData().WeaponType))
    {
        if (CurrentWeapon->GetWeaponData().WeaponType == Weapon->GetWeaponData().WeaponType)
        {
            CurrentWeapon = nullptr;
            bEquipMontageInProgress = false;
            bReloadMontageInProgress = false;
        }

        WeaponsMap[Weapon->GetWeaponData().WeaponType]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        WeaponsMap[Weapon->GetWeaponData().WeaponType]->Destroy();
    }

    WeaponsMap.Add(Weapon->GetWeaponData().WeaponType, Weapon);
    AttachToSocket(Weapon, OwnerCharacter->GetMesh(), Weapon->GetWeaponData().WeaponArmorySocketName);

    if (!CurrentWeapon)
    {
        EquipWeapon(Weapon->GetWeaponData().WeaponType);
    }

    return true;
}

void USHGBaseWeaponComponent::BindNotifys()
{
    if (const auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UShooterEquipFinishedAnimNotify>(EquipAnimMontage))
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &ThisClass::OnEquipFinished);
    }
    else
    {
        UE_LOG(WeaponComponentLog, Error, TEXT("EquipFinishedAnimNotify wasn't found!"));
        checkNoEntry();
    }
}

void USHGBaseWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    const auto OwnerCharacter = GetOwner<ACharacter>();
    if (!OwnerCharacter && OwnerCharacter->GetMesh() == MeshComp)
        return;

    bEquipMontageInProgress = false;
}

void USHGBaseWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
    const auto OwnerCharacter = GetOwner<ACharacter>();
    if (!OwnerCharacter || OwnerCharacter->GetMesh() != MeshComp)
        return;

    bReloadMontageInProgress = false;
}

void USHGBaseWeaponComponent::OnFired()
{
    OnAmmoInfoChanged.Broadcast(CurrentWeapon->GetAmmoData());
}

void USHGBaseWeaponComponent::OnClipEmpty()
{
    ReloadWeapon();
}
