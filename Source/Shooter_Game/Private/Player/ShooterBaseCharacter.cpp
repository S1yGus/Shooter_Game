// Shooter_Game, All rights reserved.

#include "Player/ShooterBaseCharacter.h"
#include "Components/SHGHealthComponent.h"
#include "Components/SHGStaminaComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "Components/ShooterBaseVFXComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShooterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SHGGameModeArena.h"

DEFINE_LOG_CATEGORY_STATIC(ShooterBaseCharacterLog, All, All)

AShooterBaseCharacter::AShooterBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USHGMovementComponent>(CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<USHGHealthComponent>("HealthComponent");
    StaminaComponent = CreateDefaultSubobject<USHGStaminaComponent>("StaminaComponent");
    WeaponComponent = CreateDefaultSubobject<USHGBaseWeaponComponent>("WeaponComponent");
    VFXComponent = CreateDefaultSubobject<USHGBaseVFXComponent>("VFXComponent");
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
    HealthComponent->OnHealthChanged.AddUObject(this, &AShooterBaseCharacter::OnHealthChanged);
    HealthComponent->OnTakeDamage.AddUObject(VFXComponent, &USHGBaseVFXComponent::SpawnImpactIndicator);
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
    if (GetCharacterMovement()->IsFalling() || !StaminaComponent->Jump())
        return;

    Super::Jump();

    if (IsSprinting())
    {
        StaminaComponent->Sprint(false);
    }
}

void AShooterBaseCharacter::SetColor(const FLinearColor& Color)
{
    const auto MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    MaterialInstance->SetVectorParameterValue(MaterialColorParameterName, Color);
}

bool AShooterBaseCharacter::IsSprinting() const
{
    return WantsToSprint && MovingForward && !GetMovementComponent()->IsFalling() && !GetVelocity().IsZero();
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
    if (!StaminaComponent->Sprint(true))
        return;

    WantsToSprint = true;

    if (IsSprinting())
    {
        WeaponComponent->StopFire();
        WeaponComponent->Zoom(false);
    }
    else
    {
        StaminaComponent->Sprint(false);
    }
}

void AShooterBaseCharacter::StopSprint()
{
    WantsToSprint = false;
    StaminaComponent->Sprint(false);
}

void AShooterBaseCharacter::OnHealthChanged(float Health, float HealthPercent)
{
}

void AShooterBaseCharacter::OnDeath(AController* KillerController, AController* VictimController)
{
    VFXComponent->PlayDeathSound();

    WeaponComponent->StopFire();

    GetCharacterMovement()->DisableMovement();

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    SetLifeSpan(LifeSpanOnDeath);

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeArena>())
    {
        GameMode->Killed(KillerController, VictimController);
    }
}

void AShooterBaseCharacter::OnOutOfStamina()
{
    StopSprint();
}

void AShooterBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    if (WantsToSprint && MovingForward)
    {
        StaminaComponent->Sprint(true);
    }

    const auto LandedVelocityZ = -GetVelocity().Z;

    if (LandedVelocityZ < LandedDamageVelocity.X)
        return;

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamageValue, LandedVelocityZ);

    TakeDamage(FinalDamage, FDamageEvent(), nullptr, nullptr);
}
