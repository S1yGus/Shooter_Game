// Shooter_Game, All rights reserved.

#include "Player/ShooterBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AShooterBaseCharacter::AShooterBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<UShooterHealthComponent>("HealthComponent");

    HealthTextRender = CreateDefaultSubobject<UTextRenderComponent>("TextRender");
    HealthTextRender->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void AShooterBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextRender);
    check(GetCharacterMovement());

    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &AShooterBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddDynamic(this, &AShooterBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &AShooterBaseCharacter::OnGroundLanded);
}

// Called every frame
void AShooterBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AShooterBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AShooterBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &AShooterBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookRight", this, &AShooterBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AShooterBaseCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AShooterBaseCharacter::StopSprint);
}

bool AShooterBaseCharacter::IsSprinting() const
{
    if (WantsToSprint && MovingForward && !GetVelocity().IsZero())
    {
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

void AShooterBaseCharacter::MoveForward(float Amount)
{
    MovingForward = Amount > 0;

    if (Amount == 0.0f)
        return;

    AddMovementInput(GetActorForwardVector(), Amount);
}

void AShooterBaseCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f || IsSprinting())
        return;

    AddMovementInput(GetActorRightVector(), Amount);
}

void AShooterBaseCharacter::StartSprint()
{
    WantsToSprint = true;
}

void AShooterBaseCharacter::StopSprint()
{
    WantsToSprint = false;
}

void AShooterBaseCharacter::OnDeath()
{
    PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(LifeSpanOnDeath);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void AShooterBaseCharacter::OnHealthChanged(float Health)
{
    HealthTextRender->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void AShooterBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto LandedVelocityZ = -GetVelocity().Z;

    if (LandedVelocityZ < LandedDamageVelocity.X)
        return;

    const auto FinalDamage =
        FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamageValue, LandedVelocityZ);

    TakeDamage(FinalDamage, FDamageEvent(), nullptr, nullptr);
}
