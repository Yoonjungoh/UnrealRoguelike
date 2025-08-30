// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/LoginGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Engine/Engine.h"

namespace
{
    // Item 2: #define보다 const, enum, inline을 선호하라
    // constexpr: 컴파일 타임에 값이 결정됨
    // const: 런타임에 변경 불가
    constexpr const TCHAR* LoginPanelPath = TEXT("/Game/ArenaBattle/UI/WBP_LoginPanel.WBP_LoginPanel_C");
    constexpr const TCHAR* ButtonName = TEXT("Button_GoToIngame");
    constexpr const TCHAR* TargetLevelName = TEXT("Ingame");
}

ALoginGameMode::ALoginGameMode()
{
    // Item 4: 객체를 사용하기 전에 반드시 그 객체를 초기화하라
    LoginPanelInstance = nullptr;

    static const ConstructorHelpers::FClassFinder<UUserWidget> LoginPanelClassRef(LoginPanelPath);
    // Item 28: 예외에 안전한 코드를 작성하라
    if (LoginPanelClassRef.Class) 
    {
        LoginPanelClass = LoginPanelClassRef.Class;
    }
}

void ALoginGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Item 21: 함수에서 객체를 반환해야 할 때는 참조자를 반환하려 들지 말라
    // -> 지역 객체나 새로 생성된 객체를 참조자(&)로 반환하면 안 된다는 의미 
    CreateLoginPanel();
}

void ALoginGameMode::CreateLoginPanel()
{
    // Item 28: 예외에 안전한 코드를 작성하라 - 단계별 검증
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

    // 성공적으로 생성된 경우에만 초기화 진행
    LoginPanelInstance->AddToViewport();
    SetupPlayerInput(PlayerController);
    BindGoToIngameButton();

    UE_LOG(LogTemp, Log, TEXT("LoginPanel created and initialized successfully"));
}

// Item 3: 가능하면 const를 사용하라
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
    // Item 28: 예외에 안전한 코드를 작성하라
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