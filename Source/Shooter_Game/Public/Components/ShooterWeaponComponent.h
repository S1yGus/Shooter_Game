// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCoreTypes.h"
#include "ShooterWeaponComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

class AShooterBaseWeaponActor;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API UShooterWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UShooterWeaponComponent();

    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void StartFire();
    void StopFire();
    void Zoom(bool Condition);
    bool IsZooing() const { return ZoomingNow; };

    virtual void NextWeapon();
    void EquipWeapon(EWeaponType WeapontType);
    void ReloadWeapon();
    bool TryToAddAmmo(TSubclassOf<AShooterBaseWeaponActor> WeaponClass, int32 ClipsAmount);
    bool TryToAddWeapon(const FWeaponData& NewWeaponData);

    bool GetCurrentWeaponUIData(FWeaponUIData& Data);
    bool GetCurrentWeaponAmmoData(FAmmoData& Data);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponEquipSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    int32 CurrentWeaponIndex = 0;

    UPROPERTY()
    AShooterBaseWeaponActor* CurrentWeapon = nullptr;

    UPROPERTY()
    TMap<EWeaponType, AShooterBaseWeaponActor*> WeaponsMap;

    bool CanFire() const;
    bool CanEquip() const;

private:
    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    bool EquipMontageInProgress = false;
    bool ReloadMontageInProgress = false;
    bool ZoomingNow = false;

    float DefaultFOVAngle = 90.0f;
    float ZoomFOVAngle = DefaultFOVAngle;
    float TargetFOVAngle;
    FTimerHandle ZoomTimerHandle;

    APlayerController* GetPlayerController() const;
    void SpawnWeapons();
    bool SpawnWeapon(TSubclassOf<AShooterBaseWeaponActor> WeaponClass);
    void AttachToSocket(AShooterBaseWeaponActor* Weapon, USceneComponent* ScenComponent, FName SocketName);
    void PlayAnimMontage(UAnimMontage* AnimMontage);
    void BindNotifys();
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);
    bool CanReload() const;
    void OnClipEmpty();
    void ChangeClip();
    void ZoomTick();
};