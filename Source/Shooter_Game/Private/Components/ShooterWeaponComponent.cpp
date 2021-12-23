// Shooter_Game, All rights reserved.


#include "Components/ShooterWeaponComponent.h"
#include "Weapons/ShooterBaseWeaponActor.h"
#include "GameFramework/Character.h"
#include "Animations/ShooterEquipFinishedAnimNotify.h"
#include "Animations/ShooterReloadFinishedAnimNotify.h"
#include "Player/ShooterBaseCharacter.h"
#include "Animations/AnimUtils.h"
#include "..\..\Public\Components\ShooterWeaponComponent.h"

UShooterWeaponComponent::UShooterWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UShooterWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    CurrentWeapon = nullptr;

    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }

    Weapons.Empty();
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

void UShooterWeaponComponent::Zoom(bool condition)
{
    if (!CurrentWeapon)
        return;

    CurrentWeapon->Zoom(condition);
}

void UShooterWeaponComponent::NextWeapon()
{
    if (!CanEquip())
        return;

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void UShooterWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
        return;

    if (!CanEquip() || CurrentWeapon == Weapons[WeaponIndex])
        return;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    if (CurrentWeapon)
    {
        StopFire();
        Zoom(false);

        AttachToSocket(CurrentWeapon, Character->GetMesh(), CurrentWeapon->GetArmorySocketName());
    }

    CurrentWeapon = Weapons[WeaponIndex];
    AttachToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

    const auto CurrentWeaponData = WeaponData.FindByPredicate(
        [&](const FWeaponData& Data) 
        {
            return Data.WeaponClasse == CurrentWeapon->GetClass();
        });

    CurrentReloadAnimMontage = CurrentWeaponData->RaloadAnimMontage;

    EquipMontageInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void UShooterWeaponComponent::ReloadWeapon()
{
    ChangeClip();
}

void UShooterWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

    check(EquipAnimMontage);

    BindNotifys();

	SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void UShooterWeaponComponent::SpawnWeapons()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    for (const auto OneWeaponData : WeaponData)
    {
        const auto Weapon = GetWorld()->SpawnActor<AShooterBaseWeaponActor>(OneWeaponData.WeaponClasse);
        if (!Weapon)
            continue;

        Weapon->OnClipEmpty.AddUObject(this, &UShooterWeaponComponent::OnClipEmpty);
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

        AttachToSocket(Weapon, Character->GetMesh(), Weapon->GetArmorySocketName());
    }
}

void UShooterWeaponComponent::AttachToSocket(AShooterBaseWeaponActor* Weapon, USceneComponent* ScenComponent,
                                             FName SocketName)
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
        const auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UShooterReloadFinishedAnimNotify>(OneWeaponData.RaloadAnimMontage);
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

    if (AutoReload)
        CurrentWeapon->StartFire();
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
