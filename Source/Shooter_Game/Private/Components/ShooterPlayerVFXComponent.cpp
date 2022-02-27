// Shooter_Game, All rights reserved.

#include "Components/ShooterPlayerVFXComponent.h"

UShooterPlayerVFXComponent::UShooterPlayerVFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UShooterPlayerVFXComponent::MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale)
{
    const auto OwnerPawn = GetOwner<APawn>();
    if (!OwnerPawn)
        return;

    const auto PlayerController = OwnerPawn->GetController<APlayerController>();
    if (!PlayerController || !PlayerController->PlayerCameraManager)
        return;

    if (!CameraShakeClass)
        return;

    PlayerController->PlayerCameraManager->StartCameraShake(CameraShakeClass, Scale);
}

void UShooterPlayerVFXComponent::BeginPlay()
{
    Super::BeginPlay();

    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UShooterPlayerVFXComponent::OnTakeAnyDamage);

    MakeCameraShake(LocomotionCameraShakeClass);
}

void UShooterPlayerVFXComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
                                                 AActor* DamageCauser)
{
    MakeCameraShake(GetDamageCameraShakeClass);
}
