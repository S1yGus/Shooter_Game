// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHGWeaponCoreTypes.h"
#include "SHGBaseWeaponComponent.generated.h"

class ASHGBaseWeaponActor;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API USHGBaseWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USHGBaseWeaponComponent();

    FOnWeaponChangedSignature OnWeaponChanged;
    FOnAmmoInfoChangedSignature OnAmmoInfoChanged;
    FOnZoomSignature OnZoom;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void StartFire();
    void StopFire();
    void SwitchFireMode();
    void Zoom(bool bCondition, bool bForce = false);
    void SwitchFlashlight();

    virtual void NextWeapon();
    void PreviousWeapon();
    void EquipWeapon(EWeaponType WeapontType);
    void ReloadWeapon();
    bool TryToAddAmmo(TSubclassOf<ASHGBaseWeaponActor> WeaponClass, int32 ClipsAmount);
    bool TryToAddWeapon(TSubclassOf<ASHGBaseWeaponActor> WeaponClass);

    bool GetCurrentWeaponUIData(FWeaponUIData& Data) const;
    bool GetCurrentWeaponAmmoData(FAmmoData& Data) const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<TSubclassOf<ASHGBaseWeaponActor>> Weapons;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponEquipSocketName{NAME_None};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* EquipAnimMontage = nullptr;

    int32 CurrentWeaponIndex = 0;

    UPROPERTY()
    ASHGBaseWeaponActor* CurrentWeapon = nullptr;

    UPROPERTY()
    TMap<EWeaponType, ASHGBaseWeaponActor*> WeaponsMap;

    inline bool CanFire() const;
    inline bool CanEquip() const;

    bool TryToEquipWeapon(EWeaponType WeapontType);

private:
    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    bool bEquipMontageInProgress = false;
    bool bReloadMontageInProgress = false;

    FTimerHandle ZoomTimerHandle;
    float DefaultFOVAngle = 0.0f;
    float TargetFOVAngle = 0.0f;

    bool bFlashlightCondition = false;

    bool GetCurrentFOV(float& CurrentFOV);
    void ZoomUpdate();

    inline void SetFlashlight(bool bCondition);

    void ChangeClip();

    inline APlayerController* GetPlayerController() const;
    inline TArray<ASHGBaseWeaponActor*> GetWeaponsMapValueArray() const;
    inline bool CanReload() const;
    inline void PlayAnimMontage(UAnimMontage* AnimMontage);
    void AttachToSocket(ASHGBaseWeaponActor* Weapon, USceneComponent* ScenComponent, FName SocketName);

    void SpawnWeapons();
    bool SpawnWeapon(TSubclassOf<ASHGBaseWeaponActor> WeaponClass);

    void BindNotifys();

    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);
    void OnFired();
    void OnClipEmpty();
};
