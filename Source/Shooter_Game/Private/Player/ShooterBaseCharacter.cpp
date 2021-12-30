// Shooter_Game, All rights reserved.

#include "Player/ShooterBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraShakeSourceComponent.h"

AShooterBaseCharacter::AShooterBaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->TargetArmLength = 170.0f;
    SpringArmComponent->SocketOffset = FVector(0.0f, 40.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->SetFieldOfView(100);
    
    HealthComponent = CreateDefaultSubobject<UShooterHealthComponent>("HealthComponent");

    HealthTextRender = CreateDefaultSubobject<UTextRenderComponent>("TextRender");
    HealthTextRender->SetupAttachment(GetMesh());

    WeaponComponent = CreateDefaultSubobject<UShooterWeaponComponent>("WeaponComponent");
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
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UShooterWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::ReloadWeapon);

    DECLARE_DELEGATE_OneParam(FEquipWeaponSignature, EWeaponType);
    PlayerInputComponent->BindAction<FEquipWeaponSignature>(
        "FirstWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::EquipWeapon, EWeaponType::Pistol);
    PlayerInputComponent->BindAction<FEquipWeaponSignature>(
        "SecondWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::EquipWeapon, EWeaponType::Rifle);
    PlayerInputComponent->BindAction<FEquipWeaponSignature>(
        "ThirdWeapom", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::EquipWeapon, EWeaponType::Shotgun);
    PlayerInputComponent->BindAction<FEquipWeaponSignature>(
        "FourthWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::EquipWeapon, EWeaponType::Launcher);
        
    DECLARE_DELEGATE_OneParam(FZoomSignature, bool);
    PlayerInputComponent->BindAction<FZoomSignature>("Zoom", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::Zoom, true);
    PlayerInputComponent->BindAction<FZoomSignature>("Zoom", IE_Released, WeaponComponent, &UShooterWeaponComponent::Zoom, false);
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
    WeaponComponent->StopFire();
    WeaponComponent->Zoom(false);

    PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    SetLifeSpan(LifeSpanOnDeath);

    if (!Controller)
        return;

    Controller->ChangeState(NAME_Spectating);
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
