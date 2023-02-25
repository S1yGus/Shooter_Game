// Shooter_Game, All rights reserved.

#include "Player/SHGPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SHGBaseWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SHGPlayerFXComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SHGGameModeBase.h"
#include "Settings/SHGGameUserSettings.h"

constexpr static float MouseSensMultiplier = 200.0f;

ASHGPlayerCharacter::ASHGPlayerCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USHGPlayerFXComponent>("FXComponent"))
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->TargetArmLength = 120.0f;
    SpringArmComponent->SocketOffset = FVector{0.0f, 60.0f, 65.0f};

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->SetFieldOfView(90.0f);

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollision");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ASHGPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(SpringArmComponent);
    check(CameraComponent);
    check(CameraCollisionComponent);

    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCameraComponentBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnCameraComponentEndOverlap);

    if (const auto GameUserSettings = USHGGameUserSettings::Get())
    {
        SensitivitySettings = GameUserSettings->GetSensitivitySettings();
        GameUserSettings->OnSensitivityChanged.AddUObject(this, &ThisClass::OnSensitivityChanged);
    }
}

void ASHGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

void ASHGPlayerCharacter::TurnOff()
{
    Super::TurnOff();

    WeaponComponent->Zoom(false, true);
}

void ASHGPlayerCharacter::Reset()
{
    Super::Reset();

    WeaponComponent->Zoom(false, true);
}

void ASHGPlayerCharacter::OnDeath(AController* KillerController, AController* VictimController)
{
    Super::OnDeath(KillerController, VictimController);

    WeaponComponent->Zoom(false, true);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);

        if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>())
        {
            GameMode->SetGameState(EGameState::Spectating);
        }
    }
}

void ASHGPlayerCharacter::MoveForward(float Amount)
{
    if (FMath::IsNearlyZero(Amount))
    {
        bIsMovingForward = false;
        return;
    }
    else if (Amount > 0)
    {
        bIsMovingForward = true;
    }
    else
    {
        bIsMovingForward = false;
    }

    AddMovementInput(FRotator{0.0, GetControlRotation().Yaw, 0.0}.Vector(), Amount);
}

void ASHGPlayerCharacter::MoveRight(float Amount)
{
    if (FMath::IsNearlyZero(Amount) || IsSprinting())
        return;

    AddMovementInput(FRotationMatrix{FRotator{0.0, GetControlRotation().Yaw, 0.0}}.GetScaledAxis(EAxis::Y), Amount);
}

void ASHGPlayerCharacter::LookUp(float Amount)
{
    float MouseYSens = bZooming ? SensitivitySettings.MouseAimedYSens : SensitivitySettings.MouseYSens;
    AddControllerPitchInput(Amount * MouseYSens * MouseSensMultiplier * GetWorld()->GetDeltaSeconds());
}

void ASHGPlayerCharacter::LookRight(float Amount)
{
    float MouseXSens = bZooming ? SensitivitySettings.MouseAimedXSens : SensitivitySettings.MouseXSens;
    AddControllerYawInput(Amount * MouseXSens * MouseSensMultiplier * GetWorld()->GetDeltaSeconds());
}

void ASHGPlayerCharacter::OnSensitivityChanged(const FSensitivitySettings& NewSensitivitySettings)
{
    SensitivitySettings = NewSensitivitySettings;
}

void ASHGPlayerCharacter::OnCameraComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                                        AActor* OtherActor,                          //
                                                        UPrimitiveComponent* OtherComp,              //
                                                        int32 OtherBodyIndex,                        //
                                                        bool bFromSweep,                             //
                                                        const FHitResult& SweepResult)
{
    CheckCameraCollisionOverlap();
}

void ASHGPlayerCharacter::OnCameraComponentEndOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                                      AActor* OtherActor,                          //
                                                      UPrimitiveComponent* OtherComp,              //
                                                      int32 OtherBodyIndex)
{
    CheckCameraCollisionOverlap();
}

inline bool ASHGPlayerCharacter::IsMovingForward() const
{
    return bIsMovingForward && Super::IsMovingForward();
}

void ASHGPlayerCharacter::CheckCameraCollisionOverlap()
{
    const bool bIsOverlapping = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetVisibility(!bIsOverlapping, true);
}
