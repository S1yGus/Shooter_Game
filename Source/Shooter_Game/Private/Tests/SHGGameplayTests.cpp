// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/SHGGameplayTests.h"
#include "CoreMinimal.h"
#include "Tests/Utils/SHGTestsUtils.h"
#include "Misc/AutomationTest.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Pickups/SHGBasePickupActor.h"
#include "Tests/Utils/InputRecordingTypes.h"
#include "Tests/Utils/JsonUtils.h"
#include "JsonObjectConverter.h"

using namespace Tests;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPickupCanBeTakenOnJump, "Shooter_Game.Gameplay.PickupCanBeTakenOnJump",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPickupCantBeTakenOnJumpIfTooHigh, "Shooter_Game.Gameplay.PickupCantBeTakenOnJumpIfTooHigh",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllPickupsAreTakenOnMovement, "Shooter_Game.Gameplay.AllPickupsAreTakenOnMovement",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FAllPickupsAreTakenOnRecordingMovement, "Shooter_Game.Gameplay.AllPickupsAreTakenOnRecordingMovement",
                                  EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::LowPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FAllMapsShouldBeLoaded, "Shooter_Game.Gameplay.AllMapsShouldBeLoaded",
                                  EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentComand, ACharacter*, Character);
bool FJumpLatentComand::Update()
{
    if (Character)
    {
        const auto JumpActionIndex = GetActionBindingIndexByName(Character->InputComponent, "Jump", EInputEvent::IE_Pressed);
        if (JumpActionIndex != INDEX_NONE)
        {
            Character->InputComponent->GetActionBinding(JumpActionIndex).ActionDelegate.Execute(EKeys::SpaceBar);
        }
    }

    return true;
}

bool FPickupCanBeTakenOnJump::RunTest(const FString& Parameters)
{
    const LevelScope Level("/Game/Tests/PickupTestLevel1");

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const auto Player = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Player must exists.", Player))
        return false;

    TArray<AActor*> PickupActors;
    UGameplayStatics::GetAllActorsOfClass(World, ASHGBasePickupActor::StaticClass(), PickupActors);
    TestTrueExpr(PickupActors.Num() == 1);

    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentComand(Player));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> PickupActors;
            UGameplayStatics::GetAllActorsOfClass(World, ASHGBasePickupActor::StaticClass(), PickupActors);
            TestTrueExpr(PickupActors.IsEmpty());
        },
        1.5f));

    return true;
}

bool FPickupCantBeTakenOnJumpIfTooHigh::RunTest(const FString& Parameters)
{
    const LevelScope Level("/Game/Tests/PickupTestLevel2");

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const auto Player = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Player must exists.", Player))
        return false;

    TArray<AActor*> PickupActors;
    UGameplayStatics::GetAllActorsOfClass(World, ASHGBasePickupActor::StaticClass(), PickupActors);
    TestTrueExpr(PickupActors.Num() == 1);

    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentComand(Player));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> PickupActors;
            UGameplayStatics::GetAllActorsOfClass(World, ASHGBasePickupActor::StaticClass(), PickupActors);
            TestTrueExpr(PickupActors.Num() == 1);
        },
        1.5f));

    return true;
}

bool FAllPickupsAreTakenOnMovement::RunTest(const FString& Parameters)
{
    const LevelScope Level("/Game/Tests/PickupTestLevel3");

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const auto Player = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Player must exists.", Player))
        return false;

    const auto InputComponent = Player->InputComponent;
    if (!TestNotNull("Player inputComponent must exists.", InputComponent.Get()))
        return false;

    TArray<AActor*> PickupActors;
    UGameplayStatics::GetAllActorsOfClass(World, ASHGBasePickupActor::StaticClass(), PickupActors);
    TestTrueExpr(PickupActors.Num() == 8);

    const auto MoveForwardAxisIndex = GetAxisBindingIndexByName(InputComponent, "MoveForward");
    if (!TestTrue("MoveForvardAxisIndex must be valid.", MoveForwardAxisIndex != INDEX_NONE))
        return false;

    const auto MoveRightdAxisIndex = GetAxisBindingIndexByName(InputComponent, "MoveRight");
    if (!TestTrue("MoveRightdAxisIndex must be valid.", MoveRightdAxisIndex != INDEX_NONE))
        return false;

    ADD_LATENT_AUTOMATION_COMMAND(FUntilLatentCommand(
        [=]()
        {
            InputComponent->AxisBindings[MoveForwardAxisIndex].AxisDelegate.Execute(1.0f);
        },
        []()
        {
        },
        3.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentComand(Player));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.5f));
    ADD_LATENT_AUTOMATION_COMMAND(FUntilLatentCommand(
        [=]()
        {
            InputComponent->AxisBindings[MoveRightdAxisIndex].AxisDelegate.Execute(1.0f);
        },
        [=]()
        {
            TArray<AActor*> PickupActors;
            UGameplayStatics::GetAllActorsOfClass(World, ASHGBasePickupActor::StaticClass(), PickupActors);
            TestTrueExpr(PickupActors.IsEmpty());
        },
        2.0f));

    return true;
}

void FAllPickupsAreTakenOnRecordingMovement::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString Name;
        FRecMovementTestData RecMovementTestData;
    };

    const TArray<FTestData> TestData = {{"PickupTestLevel4", {"/Game/Tests/PickupTestLevel4", 10, "TestInput_PickupTestLevel4.json"}},
                                        {"PickupTestLevel5", {"/Game/Tests/PickupTestLevel5", 6, "TestInput_PickupTestLevel5.json"}}};

    for (const auto& [Name, RecMovementTestData] : TestData)
    {
        FString Cmd;
        if (!FJsonObjectConverter::UStructToJsonObjectString(RecMovementTestData, Cmd))
            continue;

        OutTestCommands.Add(Cmd);
        OutBeautifiedNames.Add(Name);
    }
}

bool FAllPickupsAreTakenOnRecordingMovement::RunTest(const FString& Parameters)
{
    FRecMovementTestData RecMovementTestData;
    if (!FJsonObjectConverter::JsonObjectStringToUStruct(Parameters, &RecMovementTestData))
        return false;

    const LevelScope Level(RecMovementTestData.LevelPath);

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const auto Player = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Player must exists.", Player))
        return false;

    const auto Controller = UGameplayStatics::GetPlayerController(World, 0);
    if (!TestNotNull("Player controller must exists.", Controller))
        return false;

    FInputData InputData;
    const FString FileName = FPaths::GameSourceDir().Append("/Shooter_Game/Public/Tests/Data/").Append(RecMovementTestData.RecInputFileName);
    if (!JsonUtils::ReadUStructFromFile(FileName, InputData) || InputData.Bindings.IsEmpty())
        return false;

    Player->SetActorTransform(InputData.InitialTransform);
    Controller->SetControlRotation(InputData.InitialTransform.Rotator());

    const auto InputComponent = Player->InputComponent;
    if (!TestNotNull("Player inputComponent must exists.", InputComponent.Get()))
        return false;

    TArray<AActor*> PickupActors;
    UGameplayStatics::GetAllActorsOfClass(World, ASHGBasePickupActor::StaticClass(), PickupActors);
    TestTrueExpr(PickupActors.Num() == RecMovementTestData.PickupsAmount);

    ADD_LATENT_AUTOMATION_COMMAND(FSimulateMovmentLatentCommand(World, InputComponent, InputData.Bindings))
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> PickupActors;
            UGameplayStatics::GetAllActorsOfClass(World, ASHGBasePickupActor::StaticClass(), PickupActors);
            TestTrueExpr(PickupActors.IsEmpty());

            return true;
        }));

    return true;
}

void FAllMapsShouldBeLoaded::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString Name;
        FString LevelPath;
    };

    const TArray<FTestData> TestData = {{"MenuLevel", "/Game/Levels/MenuLevel"},          //
                                        {"SandboxLevel", "/Game/Levels/SandboxLevel"},    //
                                        {"ArenaLevel", "/Game/Levels/ArenaLevel"}};

    for (const auto& [Name, LevelPath] : TestData)
    {
        OutBeautifiedNames.Add(Name);
        OutTestCommands.Add(LevelPath);
    }
}

bool FAllMapsShouldBeLoaded::RunTest(const FString& Parameters)
{
    const LevelScope Level(Parameters);

    return true;
}

#endif    // WITH_AUTOMATION_TESTS
