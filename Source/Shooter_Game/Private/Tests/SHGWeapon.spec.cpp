// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Utils/SHGTestsUtils.h"
#include "Weapons/SHGBaseWeaponActor.h"
#include "Weapons/SHGPistolWeaponActor.h"
#include "Weapons/SHGRifleWeaponActor.h"
#include "Weapons/SHGShotgunWeaponActor.h"
#include "Weapons/SHGLauncherWeaponActor.h"

#define CPP_WEAPON_CREATIONAL_TEST(Name, Class)                                                                                         \
    const FString ErrorMsg = FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *Class::StaticClass()->GetName()); \
    AddExpectedError(ErrorMsg, EAutomationExpectedErrorFlags::Exact);                                                                   \
    const auto* Name = World->SpawnActor<Class>(Class::StaticClass(), SpawnTransform);                                                  \
    TestNull(#Name " doesn't exist.", Name);

using namespace Tests;

namespace
{
const char* EmptyTestLevellName{"/Game/Tests/EmptyTestLevel"};
const char* PistolBlueprintName{"Blueprint'/Game/Gameplay/Weapons/BP_SHGPistolWeaponActor.BP_SHGPistolWeaponActor'"};
const char* RifleBlueprintName{"Blueprint'/Game/Gameplay/Weapons/BP_SHGRifleWeaponActor.BP_SHGRifleWeaponActor'"};
const char* ShotgunBlueprintName{"Blueprint'/Game/Gameplay/Weapons/BP_SHGShotgunWeaponActor.BP_SHGShotgunWeaponActor'"};
const char* LauncherBlueprintName{"Blueprint'/Game/Gameplay/Weapons/BP_SHGLauncherWeaponActor.BP_SHGLauncherWeaponActor'"};
const char* PlayerCharacterTestableBlueprintName{"Blueprint'/Game/Tests/BP_SHGPlayerCharacterTestable.BP_SHGPlayerCharacterTestable'"};
}    // namespace

BEGIN_DEFINE_SPEC(FWeapon, "Shooter_Game.Weapon", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
const FTransform SpawnTransform{FVector{-300.0, 0.0, 42.0}};

END_DEFINE_SPEC(FWeapon)

void FWeapon::Define()
{
    Describe("Creational",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap(EmptyTestLevellName);

                         World = GetCurrentWorld();
                         TestNotNull("World must exists.", World);
                     });

                 It("Base weapon cpp instance can't created",
                    [this]()
                    {
                        CPP_WEAPON_CREATIONAL_TEST(BaseWeapon, ASHGBaseWeaponActor);
                    });

                 It("Pistol cpp instance can't created",
                    [this]()
                    {
                        CPP_WEAPON_CREATIONAL_TEST(Pistol, ASHGPistolWeaponActor);
                    });

                 It("Rifle cpp instance can't created",
                    [this]()
                    {
                        CPP_WEAPON_CREATIONAL_TEST(Rifle, ASHGRifleWeaponActor);
                    });

                 It("Shotgun cpp instance can't created",
                    [this]()
                    {
                        CPP_WEAPON_CREATIONAL_TEST(Shotgun, ASHGShotgunWeaponActor);
                    });

                 It("Launcher cpp instance can't created",
                    [this]()
                    {
                        CPP_WEAPON_CREATIONAL_TEST(Launcher, ASHGLauncherWeaponActor);
                    });

                 It("Pistol blueprint instance can be created",
                    [this]()
                    {
                        const auto* Pistol = SpawnBlueprint<ASHGPistolWeaponActor>(World, PistolBlueprintName);
                        TestNotNull("Pistol blueprint should exists.", Pistol);
                    });

                 It("Rifle blueprint instance can be created",
                    [this]()
                    {
                        const auto* Rifle = SpawnBlueprint<ASHGRifleWeaponActor>(World, RifleBlueprintName);
                        TestNotNull("Rifle blueprint should exists.", Rifle);
                    });

                 It("Shotgun blueprint instance can be created",
                    [this]()
                    {
                        const auto* Shotgun = SpawnBlueprint<ASHGShotgunWeaponActor>(World, ShotgunBlueprintName);
                        TestNotNull("Shotgun blueprint should exists.", Shotgun);
                    });

                 It("Launcher blueprint instance can be created",
                    [this]()
                    {
                        const auto* Launcher = SpawnBlueprint<ASHGLauncherWeaponActor>(World, LauncherBlueprintName);
                        TestNotNull("Launcher blueprint should exists.", Launcher);
                    });

                 AfterEach(
                     [this]()
                     {
                         SpecCloseLevel(World);
                     });
             });
}

#endif    // WITH_AUTOMATION_TESTS
