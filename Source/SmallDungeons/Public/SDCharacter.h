// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SDCharacter.generated.h"

class USDCharacterMovementComponent;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class SMALLDUNGEONS_API ASDCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASDCharacter(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void StopJumping() override;
	
	virtual void NotifyJumpApex() override;
	
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	USDCharacterMovementComponent* GetSDCharacterMovementComponent() const { return SDCharacterMovementComponent;};
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category= Movement)
	void Move(const FVector2D& Value);

	UFUNCTION(BlueprintCallable, Category= Movement)
	void Look(const FVector2D& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Jump)
	float JumpGravityModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Jump)
	float DefaultGravityScale;
	
private:

	void ModifyJumpGravity(float Value) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = true))
	USDCharacterMovementComponent* SDCharacterMovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputMappingContext* DefaultMappingContext;

};
