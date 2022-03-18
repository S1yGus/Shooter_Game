// Shooter_Game, All rights reserved.

#include "Player/ShooterBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterStaminaComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "Components/ShooterBaseVFXComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(ShooterBaseCharacterLog, All, All)

AShooterBaseCharacter::AShooterBaseCharacter(const FObjectInitializer& ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;

    HealthComponent = CreateDefaultSubobject<UShooterHealthComponent>("HealthComponent");
    StaminaComponent = CreateDefaultSubobject<UShooterStaminaComponent>("StaminaComponent");
    WeaponComponent = CreateDefaultSubobject<UShooterWeaponComponent>("WeaponComponent");
    VFXComponent = CreateDefaultSubobject<UShooterBaseVFXComponent>("VFXComponent");
}

void AShooterBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(WeaponComponent);
    check(VFXComponent);
    check(GetCharacterMovement());
    check(GetMesh());

    HealthComponent->OnDeath.AddUObject(this, &AShooterBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddDynamic(this, &AShooterBaseCharacter::OnHealthChanged);
    StaminaComponent->OnOutOfStamina.AddUObject(this, &AShooterBaseCharacter::OnOutOfStamina);
    LandedDelegate.AddDynamic(this, &AShooterBaseCharacter::OnGroundLanded);
}

void AShooterBaseCharacter::TurnOff()
{
    Super::TurnOff();

    WeaponComponent->StopFire();
}

void AShooterBaseCharacter::Reset()
{
    Super::Reset();

    WeaponComponent->StopFire();
}

void AShooterBaseCharacter::Jump()
{
    if (GetCharacterMovement()->IsFalling() || !StaminaComponent->UsingStaminaValidCheck(JumpStaminaCost))
        return;

    Super::Jump();
}

void AShooterBaseCharacter::SetColor(const FLinearColor& Color)
{
    const auto MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    MaterialInstance->SetVectorParameterValue(MaterialColorParameterName, Color);
}

bool AShooterBaseCharacter::IsSprinting() const
{
    if (WantsToSprint && MovingForward && !GetVelocity().IsZero())
    {
        WeaponComponent->StopFire();
        GetCharacterMovement()->MaxWalkSpeed = SprintWalkSpeed;
        return true;
    }

    GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
    return false;
}

float AShooterBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
        return 0.0f;

    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

FRotator AShooterBaseCharacter::GetViewDeltaRotator() const
{
    return UKismetMathLibrary::NormalizedDeltaRotator(GetBaseAimRotation(), GetActorRotation());
}

void AShooterBaseCharacter::StartSprint()
{
    StaminaComponent->UsingStamina(true, SprintStaminaFlow);
    WantsToSprint = true;
}

void AShooterBaseCharacter::StopSprint()
{
    if (!StaminaComponent->IsStaminaInUse())
        return;

    StaminaComponent->UsingStamina(false);
    WantsToSprint = false;
}

void AShooterBaseCharacter::OnDeath()
{
    VFXComponent->PlayDeathSound();

    WeaponComponent->StopFire();

    GetCharacterMovement()->DisableMovement();

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    SetLifeSpan(LifeSpanOnDeath);

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

void AShooterBaseCharacter::OnOutOfStamina()
{
    StopSprint();
}

void AShooterBaseCharacter::OnHealthChanged(float Health)
{
}

void AShooterBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto LandedVelocityZ = -GetVelocity().Z;

    if (LandedVelocityZ < LandedDamageVelocity.X)
        return;

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamageValue, LandedVelocityZ);

    TakeDamage(FinalDamage, FDamageEvent(), nullptr, nullptr);
}
