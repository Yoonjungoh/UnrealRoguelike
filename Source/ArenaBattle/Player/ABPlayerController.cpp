// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "UI/ABHUDWidget.h"

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget> UABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C"));
	if (UABHUDWidgetRef.Class)
	{
		ABHUDWidgetClass = UABHUDWidgetRef.Class;
	}
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	ABHUDWidget = CreateWidget<UABHUDWidget>(this, ABHUDWidgetClass);
	if (ABHUDWidget)
	{
		ABHUDWidget->AddToViewport();	// 화면에 띄우도록 하는 명령어
	}
}
