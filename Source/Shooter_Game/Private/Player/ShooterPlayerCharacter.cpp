// Shooter_Game, All rights reserved.

#include "Player/ShooterPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ShooterVFXComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

AShooterPlayerCharacter::AShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer) : AShooterBaseCharacter(ObjectInitializer)
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->TargetArmLength = 170.0f;
    SpringArmComponent->SocketOffset = FVector(0.0f, 40.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->SetFieldOfView(100);

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollision");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    VFXComponent = CreateDefaultSubobject<UShooterVFXComponent>("VFXComponent");
}

void AShooterPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CameraCollisionComponent);

    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AShooterPlayerCharacter::OnCameraComponentBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AShooterPlayerCharacter::OnCameraComponentEndOverlap);
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
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::ReloadWeapon);

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

void AShooterPlayerCharacter::OnDeath()
{
    Super::OnDeath();

    if (!Controller)
        return;

    Controller->ChangeState(NAME_Spectating);
}

void AShooterPlayerCharacter::MoveForward(float Amount)
{
    MovingForward = Amount > 0;

    if (Amount == 0.0f)
        return;

    AddMovementInput(GetActorForwardVector(), Amount);
}

void AShooterPlayerCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f || IsSprinting())
        return;

    AddMovementInput(GetActorRightVector(), Amount);
}

void AShooterPlayerCharacter::StartSprint()
{
    WantsToSprint = true;
}

void AShooterPlayerCharacter::StopSprint()
{
    WantsToSprint = false;
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