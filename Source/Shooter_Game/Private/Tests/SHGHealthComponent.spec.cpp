// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Utils/SHGTestsUtils.h"
#include "Tests/SHGTestConstants.h"
#include "Components/SHGHealthComponent.h"
#include "Player/SHGPlayerCharacter.h"
#include "AI/SHGAIController.h"
#include "Player/SHGPlayerController.h"

using namespace Tests;

BEGIN_DEFINE_SPEC(FHealthComponent, "Shooter_Game.Components.HealthComponent",
                  EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
ASHGPlayerCharacter* Character;
USHGHealthComponent* HealthComponent;
const FTransform SpawnTransform{FVector{-300.0, 0.0, 42.0}};

const float MaxHealth{200.0f};
const float AutoHealThreshold{150.0f};
const float AutoHealRate{0.1f};
const float AutoHealDelay{2.0f};
const float AutoHealModifier{2.0f};

const float DamageAmount{20.0f};

END_DEFINE_SPEC(FHealthComponent)

void FHealthComponent::Define()
{
    Describe("Main functional",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap(EmptyTestLevellName);

                         World = GetCurrentWorld();
                         if (!TestNotNull("World must exists.", World))
                             return;

                         Character = SpawnBlueprintDeferred<ASHGPlayerCharacter>(World, PlayerCharacterTestableBlueprintName, SpawnTransform);
                         if (!TestNotNull("Character must exists.", Character))
                             return;

                         CallFunctionByNameWithParameters(Character, "ClearDamageModifiersMap", {});
                         const bool AutoHeal{false};
                         CallFunctionByNameWithParameters(Character,                                     //
                                                          "SetHealthData",                               //
                                                          {FString::SanitizeFloat(MaxHealth),            //
                                                           AutoHeal ? "Yes" : "No",                      //
                                                           FString::SanitizeFloat(AutoHealThreshold),    //
                                                           FString::SanitizeFloat(AutoHealRate),         //
                                                           FString::SanitizeFloat(AutoHealDelay),        //
                                                           FString::SanitizeFloat(AutoHealModifier)});

                         Character->FinishSpawning(SpawnTransform);

                         HealthComponent = Character->FindComponentByClass<USHGHealthComponent>();
                         TestNotNull("HealthComponent must exists.", HealthComponent);
                     });

                 It("Initial values",
                    [this]()
                    {
                        TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth));
                        TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealthPercent(), 1.0f));
                        TestTrueExpr(!HealthComponent->IsDead());
                    });

                 It("Getters test",
                    [this]()
                    {
                        Character->TakeDamage(DamageAmount, {}, nullptr, nullptr);

                        TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth - DamageAmount));
                        TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealthPercent(), 1.0f - DamageAmount / MaxHealth));
                    });

                 It("Try to heal",
                    [this]()
                    {
                        Character->TakeDamage(DamageAmount, {}, nullptr, nullptr);

                        TestTrueExpr(HealthComponent->TryToHeal(DamageAmount));
                        TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth));
                    });

                 It("Try to owerheal",
                    [this]()
                    {
                        Character->TakeDamage(DamageAmount, {}, nullptr, nullptr);

                        TestTrueExpr(HealthComponent->TryToHeal(MaxHealth + 1.0f));
                        TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth));
                    });

                 It("Try to heal completely healthy character",
                    [this]()
                    {
                        TestTrueExpr(!HealthComponent->TryToHeal(MaxHealth));
                    });

                 It("Try to heal with negative value",
                    [this]()
                    {
                        TestTrueExpr(!HealthComponent->TryToHeal(-1.0f));
                    });

                 It("Try to heal with zero value",
                    [this]()
                    {
                        Character->TakeDamage(DamageAmount, {}, nullptr, nullptr);

                        TestTrueExpr(!HealthComponent->TryToHeal(0.0f));
                        TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth - DamageAmount));
                    });

                 It("Character can be dead",
                    [this]()
                    {
                        Character->TakeDamage(MaxHealth + 1.0f, {}, nullptr, nullptr);

                        TestTrueExpr(HealthComponent->IsDead());
                    });

                 It("Try to heal dead character",
                    [this]()
                    {
                        Character->TakeDamage(MaxHealth + 1.0f, {}, nullptr, nullptr);

                        TestTrueExpr(!HealthComponent->TryToHeal(MaxHealth));
                    });

                 It("OnHealthChanged delegate",
                    [this]()
                    {
                        float DelegateHealth{0.0f};
                        float DelegateHealthPercent{0.0f};
                        HealthComponent->OnHealthChanged.AddLambda(
                            [&](float NewHealth, float NewHealthPercent)
                            {
                                DelegateHealth = NewHealth;
                                DelegateHealthPercent = NewHealthPercent;
                            });

                        Character->TakeDamage(DamageAmount, {}, nullptr, nullptr);

                        TestTrueExpr(FMath::IsNearlyEqual(DelegateHealth, MaxHealth - DamageAmount));
                        TestTrueExpr(FMath::IsNearlyEqual(DelegateHealthPercent, 1.0f - DamageAmount / MaxHealth));
                    });

                 It("OnTakeLocalDamage delegate don't proc with AI controller",
                    [this]()
                    {
                        float DelegateDamage{0.0f};
                        HealthComponent->OnTakeLocalDamage.AddLambda(
                            [&](float NewDamage, const FVector& NewLoacation, UPhysicalMaterial* NewPhysicalMaterial)
                            {
                                DelegateDamage = NewDamage;
                            });

                        auto* AIController = World->SpawnActor<ASHGAIController>(ASHGAIController::StaticClass());
                        Character->TakeDamage(DamageAmount, FPointDamageEvent(), AIController, nullptr);

                        TestTrueExpr(FMath::IsNearlyZero(DelegateDamage));
                    });

                 It("OnTakeLocalDamage delegate proc with player controller",
                    [this]()
                    {
                        float DelegateDamage{0.0f};
                        HealthComponent->OnTakeLocalDamage.AddLambda(
                            [&](float NewDamage, const FVector& NewLoacation, UPhysicalMaterial* NewPhysicalMaterial)
                            {
                                DelegateDamage = NewDamage;
                            });

                        auto* PC = World->SpawnActor<ASHGPlayerController>(ASHGPlayerController::StaticClass());
                        Character->TakeDamage(DamageAmount, FPointDamageEvent(), PC, nullptr);

                        TestTrueExpr(FMath::IsNearlyEqual(DelegateDamage, DamageAmount));
                    });

                 It("OnDeath delegate",
                    [this]()
                    {
                        bool bIsDead{false};
                        HealthComponent->OnDeath.AddLambda(
                            [&](AController* KillerController, AController* VictimController)
                            {
                                bIsDead = true;
                            });
                        Character->TakeDamage(MaxHealth + 1.0f, {}, nullptr, nullptr);

                        TestTrueExpr(bIsDead);
                    });

                 AfterEach(
                     [this]()
                     {
                         SpecCloseLevel(World);
                     });
             });

    Describe("Auto heal",
             [this]()
             {
                 LatentBeforeEach(
                     [this](const FDoneDelegate& DoneDelegate)
                     {
                         AutomationOpenMap(EmptyTestLevellName);

                         World = GetCurrentWorld();
                         if (!TestNotNull("World must exists.", World))
                             return;

                         Character = SpawnBlueprintDeferred<ASHGPlayerCharacter>(World, PlayerCharacterTestableBlueprintName, SpawnTransform);
                         if (!TestNotNull("Character must exists.", Character))
                             return;

                         const bool AutoHeal{true};
                         CallFunctionByNameWithParameters(Character,                                     //
                                                          "SetHealthData",                               //
                                                          {FString::SanitizeFloat(MaxHealth),            //
                                                           AutoHeal ? "Yes" : "No",                      //
                                                           FString::SanitizeFloat(AutoHealThreshold),    //
                                                           FString::SanitizeFloat(AutoHealRate),         //
                                                           FString::SanitizeFloat(AutoHealDelay),        //
                                                           FString::SanitizeFloat(AutoHealModifier)});

                         Character->FinishSpawning(SpawnTransform);

                         HealthComponent = Character->FindComponentByClass<USHGHealthComponent>();
                         if (!TestNotNull("HealthComponent must exists.", HealthComponent))
                             return;

                         DoneDelegate.Execute();
                     });

                 LatentIt("Threshold", EAsyncExecution::ThreadPool,
                          [this](const FDoneDelegate& DoneDelegate)
                          {
                              const float Damage{100.0f};
                              AsyncTask(ENamedThreads::GameThread,
                                        [this, Damage]()
                                        {
                                            Character->TakeDamage(Damage, {}, nullptr, nullptr);
                                        });

                              const float SynchDelta = 0.5f;
                              FPlatformProcess::Sleep((Damage - (MaxHealth - AutoHealThreshold)) / (AutoHealModifier / AutoHealRate) + AutoHealDelay + SynchDelta);

                              AsyncTask(ENamedThreads::GameThread,
                                        [this]()
                                        {
                                            TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), AutoHealThreshold));
                                        });

                              DoneDelegate.Execute();
                          });

                 LatentIt("Delay", EAsyncExecution::ThreadPool,
                          [this](const FDoneDelegate& DoneDelegate)
                          {
                              const float Damage{100.0f};
                              AsyncTask(ENamedThreads::GameThread,
                                        [this, Damage]()
                                        {
                                            Character->TakeDamage(Damage, {}, nullptr, nullptr);
                                        });

                              FPlatformProcess::Sleep(AutoHealDelay);

                              AsyncTask(ENamedThreads::GameThread,
                                        [this, Damage]()
                                        {
                                            TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth - Damage));
                                        });

                              DoneDelegate.Execute();
                          });

                 LatentAfterEach(
                     [this](const FDoneDelegate& DoneDelegate)
                     {
                         SpecCloseLevel(World);

                         DoneDelegate.Execute();
                     });
             });
}

#endif    // WITH_AUTOMATION_TESTS
