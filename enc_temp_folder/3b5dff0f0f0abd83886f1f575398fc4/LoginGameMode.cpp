// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/LoginGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Engine/Engine.h"

namespace
{
    // Item 2: #define���� const, enum, inline�� ��ȣ�϶�
    // constexpr: ������ Ÿ�ӿ� ���� ������
    // const: ��Ÿ�ӿ� ���� �Ұ�
    constexpr const TCHAR* LoginPanelPath = TEXT("/Game/ArenaBattle/UI/WBP_LoginPanel.WBP_LoginPanel_C");
    constexpr const TCHAR* ButtonName = TEXT("Button_GoToIngame");
    constexpr const TCHAR* TargetLevelName = TEXT("Ingame");
}

ALoginGameMode::ALoginGameMode()
{
    // Item 4: ��ü�� ����ϱ� ���� �ݵ�� �� ��ü�� �ʱ�ȭ�϶�
    LoginPanelInstance = nullptr;

    static const ConstructorHelpers::FClassFinder<UUserWidget> LoginPanelClassRef(LoginPanelPath);
    // Item 28: ���ܿ� ������ �ڵ带 �ۼ��϶�
    if (LoginPanelClassRef.Class) 
    {
        LoginPanelClass = LoginPanelClassRef.Class;
    }
}

void ALoginGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Item 21: �Լ����� ��ü�� ��ȯ�ؾ� �� ���� �����ڸ� ��ȯ�Ϸ� ���� ����
    // -> ���� ��ü�� ���� ������ ��ü�� ������(&)�� ��ȯ�ϸ� �� �ȴٴ� �ǹ� 
    CreateLoginPanel();
}

void ALoginGameMode::CreateLoginPanel()
{
    // Item 28: ���ܿ� ������ �ڵ带 �ۼ��϶� - �ܰ躰 ����
    if (!LoginPanelClass)
    {
        UE_LOG(LogTemp, Error, TEXT("LoginPanelClass is not set"));
        return;
    }

    APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController not found"));
        return;
    }

    LoginPanelInstance = CreateWidget<UUserWidget>(PlayerController, LoginPanelClass);
    if (!LoginPanelInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create LoginPanel widget"));
        return;
    }

    // ���������� ������ ��쿡�� �ʱ�ȭ ����
    LoginPanelInstance->AddToViewport();
    SetupPlayerInput(PlayerController);
    BindGoToIngameButton();

    UE_LOG(LogTemp, Log, TEXT("LoginPanel created and initialized successfully"));
}

// Item 3: �����ϸ� const�� ����϶�
void ALoginGameMode::SetupPlayerInput(APlayerController* PlayerController) const
{
    if (PlayerController == nullptr)
    {
        return;
    }

    PlayerController->SetShowMouseCursor(true);

    const FInputModeUIOnly InputMode;
    PlayerController->SetInputMode(InputMode);
}

void ALoginGameMode::BindGoToIngameButton()
{
    // Item 28: ���ܿ� ������ �ڵ带 �ۼ��϶�
    if (IsLoginPanelValid() == false)
    {
        UE_LOG(LogTemp, Error, TEXT("LoginPanelInstance is null"));
        return;
    }

    UButton* const GoToIngameButton = Cast<UButton>(LoginPanelInstance->GetWidgetFromName(ButtonName));
    if (GoToIngameButton)
    {
        GoToIngameButton->OnClicked.AddDynamic(this, &ALoginGameMode::OnGoToIngameButtonClicked);
        UE_LOG(LogTemp, Log, TEXT("Button binding successful"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Button '%s' not found in LoginPanel"), ButtonName);
    }
}

void ALoginGameMode::OnGoToIngameButtonClicked()
{
    UGameplayStatics::OpenLevel(this, FName(TargetLevelName));
}