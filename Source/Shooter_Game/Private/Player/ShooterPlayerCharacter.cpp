// Shooter_Game, All rights reserved.

#include "Player/ShooterPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SHGPlayerFXComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SHGGameModeBase.h"
#include "Settings/SHGGameUserSettings.h"

constexpr static float MouseSensMultiplier = 200.0f;

AShooterPlayerCharacter::AShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USHGPlayerFXComponent>("FXComponent"))
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

    check(SpringArmComponent);
    check(CameraComponent);
    check(CameraCollisionComponent);

    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCameraComponentBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnCameraComponentEndOverlap);

    if (const auto GameUserSettings = USHGGameUserSettings::Get())
    {
        GameUserSettings->OnSensitivityChanged.AddUObject(this, &ThisClass::OnSensitivityChanged);
        SensitivitySettings = GameUserSettings->GetSensitivitySettings();
    }
}

void AShooterPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);
    PlayerInputComponent->BindAxis("LookRight", this, &ThisClass::LookRight);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ThisClass::Jump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ThisClass::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ThisClass::StopSprint);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USHGBaseWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USHGBaseWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USHGBaseWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, WeaponComponent, &USHGBaseWeaponComponent::PreviousWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USHGBaseWeaponComponent::ReloadWeapon);
    PlayerInputComponent->BindAction("SwitchFireMode", IE_Pressed, WeaponComponent, &USHGBaseWeaponComponent::SwitchFireMode);
    PlayerInputComponent->BindAction("Flashlight", IE_Pressed, WeaponComponent, &USHGBaseWeaponComponent::SwitchFlashlight);

    DECLARE_DELEGATE_OneParam(FEquipWeaponSignature, EWeaponType);
    PlayerInputComponent->BindAction<FEquipWeaponSignature>("FirstWeapon", IE_Pressed, WeaponComponent, &USHGBaseWeaponComponent::EquipWeapon, EWeaponType::Pistol);
    PlayerInputComponent->BindAction<FEquipWeaponSignature>("SecondWeapon", IE_Pressed, WeaponComponent, &USHGBaseWeaponComponent::EquipWeapon, EWeaponType::Rifle);
    PlayerInputComponent->BindAction<FEquipWeaponSignature>("ThirdWeapom", IE_Pressed, WeaponComponent, &USHGBaseWeaponComponent::EquipWeapon, EWeaponType::Shotgun);
    PlayerInputComponent->BindAction<FEquipWeaponSignature>("FourthWeapon", IE_Pressed, WeaponComponent, &USHGBaseWeaponComponent::EquipWeapon, EWeaponType::Launcher);

    DECLARE_DELEGATE_TwoParams(FZoomSignature, bool, bool);
    PlayerInputComponent->BindAction<FZoomSignature>("Zoom", IE_Pressed, WeaponComponent, &USHGBaseWeaponComponent::Zoom, true, false);
    PlayerInputComponent->BindAction<FZoomSignature>("Zoom", IE_Released, WeaponComponent, &USHGBaseWeaponComponent::Zoom, false, false);
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

void AShooterPlayerCharacter::OnDeath(AController* KillerController, AController* VictimController)
{
    Super::OnDeath(KillerController, VictimController);

    WeaponComponent->Zoom(false, true);

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>())
    {
        GameMode->SetGameState(EGameState::Spectating);
    }

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void AShooterPlayerCharacter::MoveForward(float Amount)
{
    MovingForward = Amount > 0;

    if (FMath::IsNearlyZero(Amount))
        return;

    const auto ControllerForvardVector = UKismetMathLibrary::GetForwardVector(FRotator{0.0f, GetControlRotation().Yaw, 0.0f});
    AddMovementInput(ControllerForvardVector, Amount);
}

void AShooterPlayerCharacter::MoveRight(float Amount)
{
    if (FMath::IsNearlyZero(Amount) || IsSprinting())
        return;

    const auto ControllerRightVector = UKismetMathLibrary::GetRightVector(FRotator{0.0f, GetControlRotation().Yaw, 0.0f});
    AddMovementInput(ControllerRightVector, Amount);
}

void AShooterPlayerCharacter::LookUp(float Amount)
{
    float MouseYSens = WeaponComponent->IsZoomingNow() ? SensitivitySettings.MouseAimedYSens : SensitivitySettings.MouseYSens;
    AddControllerPitchInput(Amount * MouseYSens * MouseSensMultiplier * GetWorld()->GetDeltaSeconds());
}

void AShooterPlayerCharacter::LookRight(float Amount)
{
    float MouseXSens = WeaponComponent->IsZoomingNow() ? SensitivitySettings.MouseAimedXSens : SensitivitySettings.MouseXSens;
    AddControllerYawInput(Amount * MouseXSens * MouseSensMultiplier * GetWorld()->GetDeltaSeconds());
}

void AShooterPlayerCharacter::OnSensitivityChanged(const FSensitivitySettings& NewSensitivitySettings)
{
    SensitivitySettings = NewSensitivitySettings;
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
