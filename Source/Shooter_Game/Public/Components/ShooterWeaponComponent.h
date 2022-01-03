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
    void Zoom(bool Condition);

    void NextWeapon();
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

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    AShooterBaseWeaponActor* CurrentWeapon;

	UPROPERTY()
    TMap<EWeaponType, AShooterBaseWeaponActor*> WeaponsMap;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

	int32 CurrentWeaponIndex = 0;
    bool EquipMontageInProgress = false;
    bool ReloadMontageInProgress = false;

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
    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;
    void OnClipEmpty();
    void ChangeClip();
    void ZoomTick();
};
