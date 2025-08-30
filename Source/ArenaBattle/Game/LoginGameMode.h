// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoginGameMode.generated.h"

// Forward declarations (Item 31: ������ ������ �ּ�ȭ)
class UUserWidget;
class UButton;

UCLASS()
class ARENABATTLE_API ALoginGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALoginGameMode();

	// Item 7: ������ ��� Ŭ���������� �Ҹ��ڸ� �������� �����϶�
	virtual ~ALoginGameMode() = default;

protected:
	virtual void BeginPlay() override;

	// �α��� �г� ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> LoginPanelClass;

	// ������ ���� �ν��Ͻ� 
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> LoginPanelInstance; // UE5 ��� ����Ʈ ������ (Item 13: �ڿ� ������ ��ü�� �̿��϶�)

private:
	// Item 22: ������ ����� private���� �����϶�
	// Item 18: �������̽� ���迡�� �����Ժ��� ��Ȯ���� �߱��϶�
	UFUNCTION()
	void CreateLoginPanel();

	UFUNCTION()
	void BindGoToIngameButton();

	UFUNCTION()
	void OnGoToIngameButtonClicked();

	// Item 30: �ζ��� �Լ��� ������� �����϶�
	/*
	* 
	const noexcept �� ��������ν�,
	"�� �Լ��� ��ü�� �������� �ʰ�, ���ܵ� ������ �ʴ´�"�� 
	���� ������ �����Ͽ� �ڵ��� �������� ������ ����Ų�ٰ� ��
	*/ 
	inline bool IsLoginPanelValid() const noexcept { return LoginPanelInstance != nullptr; }

	// Item 3: �����ϸ� const�� ����϶�
	void SetupPlayerInput(class APlayerController* PlayerController) const;
};