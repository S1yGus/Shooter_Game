// Shooter_Game, All rights reserved.

#include "Components/ShooterWeaponComponent.h"
#include "Weapons/ShooterBaseWeaponActor.h"
#include "GameFramework/Character.h"
#include "Animations/ShooterEquipFinishedAnimNotify.h"
#include "Animations/ShooterReloadFinishedAnimNotify.h"
#include "Player/ShooterBaseCharacter.h"
#include "Animations/AnimUtils.h"

constexpr static float ZoomTimerRate = 0.01f;
constexpr static float ZoomChangeSpeed = 10.0f;

UShooterWeaponComponent::UShooterWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UShooterWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    CurrentWeapon = nullptr;

    TArray<AShooterBaseWeaponActor*> WeaponsArray;
    WeaponsMap.GenerateValueArray(WeaponsArray);
    for (auto Weapon : WeaponsArray)
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
    CurrentWeapon->StopFire();
}

void UShooterWeaponComponent::Zoom(bool Condition)
{
    if (!CurrentWeapon)
        return;

    const auto PlayerController = GetPlayerController();
    if (!PlayerController || !PlayerController->PlayerCameraManager)
        return;

    if (!CurrentWeapon->GetZoomFOV(ZoomFOVAngle))
        return;

    if (GetOwner<AShooterBaseCharacter>()->IsSprinting())
        return;

    ZoomingNow = Condition;
    TargetFOVAngle = Condition ? ZoomFOVAngle : DefaultFOVAngle;

    if (!ZoomTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().SetTimer(ZoomTimerHandle, this, &UShooterWeaponComponent::ZoomTick, ZoomTimerRate, true);
    }
}

void UShooterWeaponComponent::NextWeapon()
{
    if (!CanEquip() || WeaponsMap.Num() == 0)
        return;

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % StaticCast<int32>(EWeaponType::Max);
    if (WeaponsMap.Contains(StaticCast<EWeaponType>(CurrentWeaponIndex)))
    {
        EquipWeapon(StaticCast<EWeaponType>(CurrentWeaponIndex));
        return;
    }

    NextWeapon();
}

void UShooterWeaponComponent::EquipWeapon(EWeaponType WeapontType)
{
    if (!WeaponsMap.Contains(WeapontType) || !CanEquip())
        return;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    if (CurrentWeapon)
    {
        if (CurrentWeapon->GetWeaponType() == WeapontType)
            return;

        StopFire();
        Zoom(false);

        AttachToSocket(CurrentWeapon, Character->GetMesh(), CurrentWeapon->GetArmorySocketName());
    }

    CurrentWeapon = WeaponsMap[WeapontType];
    AttachToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

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
    TArray<AShooterBaseWeaponActor*> WeaponsArray;
    WeaponsMap.GenerateValueArray(WeaponsArray);
    for (const auto Weapon : WeaponsArray)
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
    TArray<AShooterBaseWeaponActor*> WeaponsArray;
    WeaponsMap.GenerateValueArray(WeaponsArray);
    for (const auto Weapon : WeaponsArray)
    {
        if (Weapon->GetClass() == NewWeaponData.WeaponClass)
        {
            return false;
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

bool UShooterWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& Data)
{
    if (!CurrentWeapon)
        return false;

    Data = CurrentWeapon->GetUIData();
    return true;
}

bool UShooterWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& Data)
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
    EquipWeapon(EWeaponType::Pistol);

    const auto PlayerController = GetPlayerController();
    if (PlayerController && PlayerController->PlayerCameraManager)
    {
        DefaultFOVAngle = PlayerController->PlayerCameraManager->GetFOVAngle();
    }
}

APlayerController* UShooterWeaponComponent::GetPlayerController() const
{
    const auto PlayerPawn = Cast<APawn>(GetOwner());
    if (!PlayerPawn)
        return nullptr;

    return Cast<APlayerController>(PlayerPawn->GetController());
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
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return false;

    const auto Weapon = GetWorld()->SpawnActor<AShooterBaseWeaponActor>(WeaponClass);
    if (!Weapon)
        return false;

    Weapon->OnClipEmpty.AddUObject(this, &UShooterWeaponComponent::OnClipEmpty);
    Weapon->SetOwner(Character);
    WeaponsMap.Add(Weapon->GetWeaponType(), Weapon);

    AttachToSocket(Weapon, Character->GetMesh(), Weapon->GetArmorySocketName());

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
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    Character->PlayAnimMontage(AnimMontage);
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
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character && Character->GetMesh() == MeshComp)
        return;

    EquipMontageInProgress = false;
}

void UShooterWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComp)
        return;

    ReloadMontageInProgress = false;
}

bool UShooterWeaponComponent::CanFire() const
{
    AShooterBaseCharacter* Character = Cast<AShooterBaseCharacter>(GetOwner());
    if (!Character)
        return false;

    return CurrentWeapon && !EquipMontageInProgress && !ReloadMontageInProgress && !Character->IsSprinting();
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

void UShooterWeaponComponent::ZoomTick()
{
    const auto PlayerController = GetPlayerController();
    if (!PlayerController)
        return;

    const float NewFOVAngle = FMath::FInterpTo(PlayerController->PlayerCameraManager->GetFOVAngle(), TargetFOVAngle, ZoomTimerRate, ZoomChangeSpeed);
    PlayerController->PlayerCameraManager->SetFOV(NewFOVAngle);

    if (FMath::IsNearlyEqual(NewFOVAngle, TargetFOVAngle))
    {
        GetWorld()->GetTimerManager().ClearTimer(ZoomTimerHandle);
    }
}
