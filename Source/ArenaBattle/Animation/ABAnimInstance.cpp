// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ABAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UABAnimInstance::UABAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

bool UABAnimInstance::IsIdle()
{
	return GroundSpeed < MovingThreshold;
}

bool UABAnimInstance::IsJumping()
{
	return bIsFalling && (Velocity.Z > JumpingThreshold);
}

void UABAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Owner = Cast<ACharacter>(GetOwningActor());	// 반환 타입 액터라 캐스팅 해줌
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();	// x, y만의 크기
		bIsIdle = IsIdle();
		bIsFalling = Movement->IsFalling();
		bIsJumping = IsJumping();
	}
}
