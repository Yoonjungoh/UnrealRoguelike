// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()
public:
	AABAIController();

	void RunAI();
	void StopAI();

protected:
	// 컨트롤러가 폰에 빙의해서 조종할 때, 발생하는 이벤트 함수
	virtual void OnPossess(APawn* InPawn) override;	

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

};
