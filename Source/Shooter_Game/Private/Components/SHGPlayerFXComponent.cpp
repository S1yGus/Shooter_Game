// Shooter_Game, All rights reserved.

#include "Components/SHGPlayerFXComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "VFX/SHGLocomotionCameraShake.h"
#include "Kismet/KismetMathLibrary.h"

constexpr static float LocomotionCameraShakeRate = 0.08f;

USHGPlayerFXComponent::USHGPlayerFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USHGPlayerFXComponent::BeginPlay()
{
    Super::BeginPlay();

    if (const auto Owner = GetOwner<APawn>())
    {
        check(GetDamageCameraShakeClass);

        Owner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamage);
        Owner->ReceiveControllerChangedDelegate.AddDynamic(this, &ThisClass::OnPossess);
    }
}

APlayerCameraManager* USHGPlayerFXComponent::GetPlayerCameraManager() const
{
    const auto OwnerPawn = GetOwner<APawn>();
    if (!OwnerPawn)
        return nullptr;

    const auto PC = OwnerPawn->GetController<APlayerController>();
    if (!PC)
        return nullptr;

    return PC->PlayerCameraManager;
}

float USHGPlayerFXComponent::GetPlayerVelocityNormalized() const
{
    if (const auto OwnerPawn = GetOwner<APawn>())
    {
        return UKismetMathLibrary::NormalizeToRange(OwnerPawn->GetVelocity().Size(), PlayerSpeedRange.X, PlayerSpeedRange.Y);
    }

    return 0.0f;
}

void USHGPlayerFXComponent::InitLocomotionCameraShake()
{
    if (const auto PlayerCameraManager = GetPlayerCameraManager())
    {
        PlayerCameraManager->StopAllCameraShakes();
        LocomotionCameraShake = Cast<USHGLocomotionCameraShake>(PlayerCameraManager->StartCameraShake(LocomotionCameraShakeClass));
        check(LocomotionCameraShake);

        GetWorld()->GetTimerManager().SetTimer(LocomotionCameraShakeTimerHandle, this, &ThisClass::OnUpdateLocomotionCameraShake, LocomotionCameraShakeRate, true);
    }

    if (const auto OwnerPawn = GetOwner())
    {
        if (const auto WeaponComponent = OwnerPawn->FindComponentByClass<USHGBaseWeaponComponent>())
        {
            WeaponComponent->OnZoom.AddLambda(
                [&](bool Condition)
                {
                    bZoom = Condition;
                });
        }
    }
}

void USHGPlayerFXComponent::MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale)
{
    if (const auto PlayerCameraManager = GetPlayerCameraManager())
    {
        PlayerCameraManager->StartCameraShake(CameraShakeClass, Scale);
    }
}

void USHGPlayerFXComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    const auto Scale = FMath::GetMappedRangeValueClamped(CameraShakeDamageRange, FVector2D{0.0f, 1.0f}, Damage);    // Scale from 0.0f to 1.0f.
    MakeCameraShake(GetDamageCameraShakeClass, Scale);
}

void USHGPlayerFXComponent::OnPossess(APawn* Pawn, AController* OldController, AController* NewController)
{
    if (bLocomotionCameraShakeEnabled)
    {
        check(LocomotionCameraShakeClass);

        InitLocomotionCameraShake();
    }
}

void USHGPlayerFXComponent::OnUpdateLocomotionCameraShake()
{
    const auto OwnerPawn = GetOwner<APawn>();
    if (!OwnerPawn)
        return;

    const auto MovementComponent = OwnerPawn->GetMovementComponent();
    if (!MovementComponent)
        return;

    float RotationAmplitudeMultiplier, RotationFrequencyMultiplier;
    RotationAmplitudeMultiplier = RotationFrequencyMultiplier = MovementComponent->IsFalling() ? 0.0f : GetPlayerVelocityNormalized();

    const auto AmplitudeMultiplier = bZoom ? AmplitudeMultiplierOnZoom : 1.0f;
    LocomotionCameraShake->UpdateLocomotionCameraShake(AmplitudeMultiplier * RotationAmplitudeMultiplier, RotationFrequencyMultiplier);
}
