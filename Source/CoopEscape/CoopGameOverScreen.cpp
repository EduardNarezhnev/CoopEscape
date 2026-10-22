#include "CoopGameOverScreen.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "CoopPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void UCoopGameOverScreen::NativeConstruct()
{
    Super::NativeConstruct();

    if(Button_Restart)
    {
        Button_Restart->OnClicked.AddDynamic(this, &UCoopGameOverScreen::OnRestartClicked);
    }
    if(Button_Exit)
    {
        Button_Exit->OnClicked.AddDynamic(this, &UCoopGameOverScreen::OnExitClicked);
    }
}

void UCoopGameOverScreen::SetResult(bool bWon)
{
    if(ResultText)
    {
        ResultText->SetText(FText::FromString(bWon ? TEXT("YOU WIN!") : TEXT("YOU LOSE!")));
    }
}

void UCoopGameOverScreen::OnRestartClicked()
{
    if (ACoopPlayerController* PC = Cast<ACoopPlayerController>(GetOwningPlayer()))
    {
        PC->Server_RestartGame();
    }
}

void UCoopGameOverScreen::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}