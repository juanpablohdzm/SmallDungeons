// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SDCharacterMovementComponent.h"


// Sets default values for this component's properties
USDCharacterMovementComponent::USDCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	JumpGravityModifier = 10.0f;
	JumpZVelocity = 900.0f;
	GravityScale = 8.0f;
	MaxWalkSpeed = 850.0f;
	RotationRate = FRotator(0.0f, 900.0f, 0.0f);
}

void USDCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	DefaultGravityScale = GravityScale;
}

void USDCharacterMovementComponent::StopJumping()
{
	if (IsFalling())
	{
		ModifyJumpGravity(JumpGravityModifier);
	}
}

void USDCharacterMovementComponent::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	ModifyJumpGravity(JumpGravityModifier);
}

void USDCharacterMovementComponent::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	bOrientRotationToMovement = !IsFalling();
	
	if (PrevMovementMode == MOVE_Falling && !IsFalling())
	{
		ModifyJumpGravity(DefaultGravityScale);
		bNotifyApex = true;
	}
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
}

void USDCharacterMovementComponent::ModifyJumpGravity(float Value)
{
	GravityScale = Value;
}
