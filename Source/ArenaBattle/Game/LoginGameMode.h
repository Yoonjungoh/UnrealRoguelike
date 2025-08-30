// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoginGameMode.generated.h"

// Forward declarations (Item 31: 컴파일 의존성 최소화)
class UUserWidget;
class UButton;

UCLASS()
class ARENABATTLE_API ALoginGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALoginGameMode();

	// Item 7: 다형성 기반 클래스에서는 소멸자를 가상으로 선언하라
	virtual ~ALoginGameMode() = default;

protected:
	virtual void BeginPlay() override;

	// 로그인 패널 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> LoginPanelClass;

	// 생성된 위젯 인스턴스 
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> LoginPanelInstance; // UE5 방식 스마트 포인터 (Item 13: 자원 관리에 객체를 이용하라)

private:
	// Item 22: 데이터 멤버를 private으로 선언하라
	// Item 18: 인터페이스 설계에는 완전함보다 정확함을 추구하라
	UFUNCTION()
	void CreateLoginPanel();

	UFUNCTION()
	void BindGoToIngameButton();

	UFUNCTION()
	void OnGoToIngameButtonClicked();

	// Item 30: 인라인 함수의 장단점을 이해하라
	/*
	* 
	const noexcept 를 사용함으로써,
	"이 함수는 객체를 수정하지 않고, 예외도 던지지 않는다"는 
	강한 보장을 제공하여 코드의 안전성과 성능을 향상시킨다고 함
	*/ 
	inline bool IsLoginPanelValid() const noexcept { return LoginPanelInstance != nullptr; }

	// Item 3: 가능하면 const를 사용하라
	void SetupPlayerInput(class APlayerController* PlayerController) const;
};