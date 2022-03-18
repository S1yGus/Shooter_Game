// Shooter_Game, All rights reserved.

#include "Player/ShooterPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShooterPlayerVFXComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ShooterStaminaComponent.h"

constexpr static float CharacterRotationSpeed = 1.5f;
constexpr static float StartTurnAngle = 30.0f;

AShooterPlayerCharacter::AShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UShooterPlayerVFXComponent>("VFXComponent"))
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->TargetArmLength = 140.0f;
    SpringArmComponent->SocketOffset = FVector(0.0f, 55.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->SetFieldOfView(100);

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollision");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void AShooterPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CameraCollisionComponent);

    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AShooterPlayerCharacter::OnCameraComponentBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AShooterPlayerCharacter::OnCameraComponentEndOverlap);
}

void AShooterPlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (FMath::Abs(GetViewDeltaRotator().Yaw) > StartTurnAngle)
    {
        const auto CurrentRotation = GetActorRotation();
        const auto NewRotation = FMath::RInterpTo(CurrentRotation, GetBaseAimRotation(), DeltaSeconds, CharacterRotationSpeed);
        SetActorRotation(FRotator(CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll));
    }
}

void AShooterPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AShooterPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AShooterPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &AShooterPlayerCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookRight", this, &AShooterPlayerCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterPlayerCharacter::Jump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AShooterPlayerCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AShooterPlayerCharacter::StopSprint);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UShooterWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::PreviousWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::ReloadWeapon);
    PlayerInputComponent->BindAction("SwitchFireMode", IE_Released, WeaponComponent, &UShooterWeaponComponent::SwitchFireMode);

    DECLARE_DELEGATE_OneParam(FEquipWeaponSignature, EWeaponType);
    PlayerInputComponent->BindAction<FEquipWeaponSignature>("FirstWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::EquipWeapon,
                                                            EWeaponType::Pistol);
    PlayerInputComponent->BindAction<FEquipWeaponSignature>("SecondWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::EquipWeapon,
                                                            EWeaponType::Rifle);
    PlayerInputComponent->BindAction<FEquipWeaponSignature>("ThirdWeapom", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::EquipWeapon,
                                                            EWeaponType::Shotgun);
    PlayerInputComponent->BindAction<FEquipWeaponSignature>("FourthWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::EquipWeapon,
                                                            EWeaponType::Launcher);

    DECLARE_DELEGATE_OneParam(FZoomSignature, bool);
    PlayerInputComponent->BindAction<FZoomSignature>("Zoom", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::Zoom, true);
    PlayerInputComponent->BindAction<FZoomSignature>("Zoom", IE_Released, WeaponComponent, &UShooterWeaponComponent::Zoom, false);
}

void AShooterPlayerCharacter::TurnOff()
{
    Super::TurnOff();

    WeaponComponent->Zoom(false);
}

void AShooterPlayerCharacter::Reset()
{
    Super::Reset();

    WeaponComponent->Zoom(false);
}

void AShooterPlayerCharacter::OnDeath()
{
    Super::OnDeath();

    WeaponComponent->Zoom(false);

    if (!Controller)
        return;

    Controller->ChangeState(NAME_Spectating);
}

void AShooterPlayerCharacter::MoveForward(float Amount)
{
    MovingForward = Amount > 0;

    if (FMath::IsNearlyZero(Amount))
        return;
    const auto ControllerForvardVector = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
    AddMovementInput(ControllerForvardVector, Amount);
}

void AShooterPlayerCharacter::MoveRight(float Amount)
{
    if (FMath::IsNearlyZero(Amount) || IsSprinting())
        return;

    const auto ControllerRightVector = UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
    AddMovementInput(ControllerRightVector, Amount);
}

void AShooterPlayerCharacter::OnCameraComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                                            AActor* OtherActor,                          //
                                                            UPrimitiveComponent* OtherComp,              //
                                                            int32 OtherBodyIndex,                        //
                                                            bool bFromSweep,                             //
                                                            const FHitResult& SweepResult)
{
    CheckCameraCollisionOverlap();
}

void AShooterPlayerCharacter::OnCameraComponentEndOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                                          AActor* OtherActor,                          //
                                                          UPrimitiveComponent* OtherComp,              //
                                                          int32 OtherBodyIndex)
{
    CheckCameraCollisionOverlap();
}

void AShooterPlayerCharacter::CheckCameraCollisionOverlap()
{
    const bool IsOverlapping = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());

    GetMesh()->SetVisibility(!IsOverlapping, true);
}
