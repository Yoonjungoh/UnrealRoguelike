// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoginPanelUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

namespace
{
    // Item 2: #define���� const, enum, inline�� ��ȣ�϶�
    // constexpr: ������ Ÿ�ӿ� ���� ������
    // const: ��Ÿ�ӿ� ���� �Ұ�
    constexpr const TCHAR* TargetLevelName = TEXT("Ingame");
}

// ������ �ʱ�ȭ �Լ�
void ULoginPanelUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // meta=(BindWidget)�� ����ϸ� Blueprint�� ������ �ڵ����� �����
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
