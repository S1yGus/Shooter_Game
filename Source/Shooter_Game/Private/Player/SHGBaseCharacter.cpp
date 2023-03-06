// Shooter_Game, All rights reserved.

#include "Player/SHGBaseCharacter.h"
#include "Components/SHGHealthComponent.h"
#include "Components/SHGStaminaComponent.h"
#include "Components/SHGBaseWeaponComponent.h"
#include "Components/SHGBaseFXComponent.h"
#include "Components/SHGMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SHGGameModeArena.h"

DEFINE_LOG_CATEGORY_STATIC(SHGBaseCharacterLog, All, All)

ASHGBaseCharacter::ASHGBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USHGMovementComponent>(CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<USHGHealthComponent>("HealthComponent");
    StaminaComponent = CreateDefaultSubobject<USHGStaminaComponent>("StaminaComponent");
    WeaponComponent = CreateDefaultSubobject<USHGBaseWeaponComponent>("WeaponComponent");
    FXComponent = CreateDefaultSubobject<USHGBaseFXComponent>("FXComponent");
    NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>("NoiseEmitterComponent");
}

void ASHGBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(StaminaComponent);
    check(WeaponComponent);
    check(FXComponent);
    check(GetCharacterMovement());
    check(GetMesh());

    HealthComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);
    HealthComponent->OnTakeLocalDamage.AddUObject(FXComponent, &USHGBaseFXComponent::SpawnImpactIndicator);
    StaminaComponent->OnOutOfStamina.AddUObject(this, &ThisClass::OnOutOfStamina);
    WeaponComponent->OnZoom.AddUObject(this, &ThisClass::OnZoom);
    LandedDelegate.AddDynamic(this, &ThisClass::OnGroundLanded);
}

void ASHGBaseCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    bCurrentSprintState = WantsToSprint();
    if (bCurrentSprintState != bLastSprintState)
    {
        WeaponComponent->StopFire();
        WeaponComponent->Zoom(false);

        bLastSprintState = bCurrentSprintState;
    }
}

void ASHGBaseCharacter::TurnOff()
{
    Super::TurnOff();

    WeaponComponent->StopFire();
}

void ASHGBaseCharacter::Reset()
{
    Super::Reset();

    WeaponComponent->StopFire();
}

void ASHGBaseCharacter::Jump()
{
    if (GetCharacterMovement()->IsFalling() || !StaminaComponent->Jump())
        return;

    Super::Jump();
}

void ASHGBaseCharacter::SetColor(const FLinearColor& Color)
{
    auto MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    MaterialInstance->SetVectorParameterValue(MaterialColorParameterName, Color);
}

float ASHGBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
        return 0.0f;

    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal)));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    return CrossProduct.IsZero() ? AngleBetween : AngleBetween * FMath::Sign(CrossProduct.Z);
}

FRotator ASHGBaseCharacter::GetViewDeltaRotator() const
{
    return UKismetMathLibrary::NormalizedDeltaRotator(GetBaseAimRotation(), GetActorRotation());
}

bool ASHGBaseCharacter::WantsToSprint() const
{
    return bWantsToSprint && IsMovingForward() && !GetMovementComponent()->IsFalling();
}

bool ASHGBaseCharacter::IsMovingForward() const
{
    return FVector::DotProduct(GetActorForwardVector(), GetVelocity().GetSafeNormal()) > 0.1f;    // Some small number greater than 0 to avoid issues during the sprint.
}

void ASHGBaseCharacter::StartSprint()
{
    if (StaminaComponent->CanSprint())
    {
        bWantsToSprint = true;
    }
}

void ASHGBaseCharacter::StopSprint()
{
    bWantsToSprint = false;
}

void ASHGBaseCharacter::OnDeath(AController* KillerController, AController* VictimController)
{
    WeaponComponent->StopFire();

    GetCharacterMovement()->DisableMovement();

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    SetLifeSpan(LifeSpanOnDeath);

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);

    FXComponent->PlayDeathSound();

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeArena>())
    {
        GameMode->Killed(KillerController, VictimController);
    }
}

void ASHGBaseCharacter::OnOutOfStamina()
{
    StopSprint();
}

void ASHGBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    if (const auto LandedVelocityZ = -GetVelocity().Z; LandedVelocityZ >= LandedDamageVelocity.X)
    {
        TakeDamage(FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, LandedVelocityZ), FDamageEvent{}, GetController(), this);
    }
}

void ASHGBaseCharacter::OnZoom(bool bCondition)
{
    bZooming = bCondition;
}
