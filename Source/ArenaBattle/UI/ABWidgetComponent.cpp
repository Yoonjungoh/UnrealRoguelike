// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
#include "ABUserWidget.h"

void UABWidgetComponent::InitWidget()
{
	Super::InitWidget();
	// 위의 Super 이후에 위젯에 대한 인스턴스가 생성됨 => 객체가 존재한다고 판다 할 수 있음
	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner());
	}
	
}
