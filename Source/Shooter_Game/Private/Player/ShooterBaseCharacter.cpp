// Shooter_Game, All rights reserved.

#include "Player/ShooterBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

// Called when the game starts or when spawned
void AShooterBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
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
        GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
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
