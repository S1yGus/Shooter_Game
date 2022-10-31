// Shooter_Game, All rights reserved.

#include "SHGGameInstance.h"
#include "Kismet/GameplayStatics.h"

void USHGGameInstance::OpenMainMenu()
{
    if (MenuLevelName == NAME_None)
        return;

    UGameplayStatics::OpenLevel(this, MenuLevelName);
}

void USHGGameInstance::StartGame()
{
    if (NewGameLevelData.LevelName.IsNone())
        return;

    UGameplayStatics::OpenLevel(this, NewGameLevelData.LevelName);
}

void USHGGameInstance::RestartLevel()
{
    UGameplayStatics::OpenLevel(this, FName{UGameplayStatics::GetCurrentLevelName(this)});
}

void USHGGameInstance::QuitGame(APlayerController* SpecificPlayer)
{
    UKismetSystemLibrary::QuitGame(this, SpecificPlayer, EQuitPreference::Quit, true);
}

void USHGGameInstance::OnStart()
{
    Super::OnStart();
}
