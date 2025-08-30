// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginPanelUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API ULoginPanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY()
    class UButton* Button_GoToIngame;

private:
    UFUNCTION()
    void OnGoToIngameButtonClicked();
};
