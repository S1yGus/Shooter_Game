// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCoreTypes.h"
#include "ShooterWeaponComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

class AShooterBaseWeaponActor;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_GAME_API UShooterWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShooterWeaponComponent();

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void StartFire();
    void StopFire();
    void Zoom(bool condition);

    void NextWeapon();
    void EquipWeapon(int32 WeaponIndex);
    void ReloadWeapon();

    bool GetCurrentWeaponUIData(FWeaponUIData& Data);
    bool GetCurrentWeaponAmmoData(FAmmoData& Data);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool AutoReload = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponEquipSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

private:
    UPROPERTY()
    AShooterBaseWeaponActor* CurrentWeapon;

	UPROPERTY()
    TArray<AShooterBaseWeaponActor*> Weapons;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

	int32 CurrentWeaponIndex = 0;
    bool EquipMontageInProgress = false;
    bool ReloadMontageInProgress = false;

    void SpawnWeapons();
    void AttachToSocket(AShooterBaseWeaponActor* Weapon, USceneComponent* ScenComponent, FName SocketName);
    void PlayAnimMontage(UAnimMontage* AnimMontage);
    void BindNotifys();
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);
    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;
    void OnClipEmpty();
    void ChangeClip();
};
