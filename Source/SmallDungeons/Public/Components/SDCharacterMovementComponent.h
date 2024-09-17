// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SDCharacterMovementComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMALLDUNGEONS_API USDCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USDCharacterMovementComponent();

	virtual void BeginPlay() override;
	
	virtual void StopJumping();
	
	virtual void NotifyJumpApex() override;
	
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	void ModifyJumpGravity(float Value);
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Jump, meta = (AllowPrivateAccess = true))
	float JumpGravityModifier;
	
	float DefaultGravityScale;
};
