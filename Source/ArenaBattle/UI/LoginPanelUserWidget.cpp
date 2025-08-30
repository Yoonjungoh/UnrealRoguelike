// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoginPanelUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

namespace
{
    // Item 2: #define보다 const, enum, inline을 선호하라
    // constexpr: 컴파일 타임에 값이 결정됨
    // const: 런타임에 변경 불가
    constexpr const TCHAR* TargetLevelName = TEXT("Ingame");
}

// 위젯의 초기화 함수
void ULoginPanelUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // meta=(BindWidget)를 사용하면 Blueprint의 위젯과 자동으로 연결됨
    if (Button_GoToIngame)
    {
        Button_GoToIngame->OnClicked.AddDynamic(this, &ULoginPanelUserWidget::OnGoToIngameButtonClicked);
        UE_LOG(LogTemp, Log, TEXT("Button_GoToIngame binding successful."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Button_GoToIngame is not bound. Check the widget blueprint."));
    }
}

void ULoginPanelUserWidget::OnGoToIngameButtonClicked()
{
    UGameplayStatics::OpenLevel(this, TargetLevelName);
}
