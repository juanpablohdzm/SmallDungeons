// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "SDCharacter.generated.h"

class USDCharacterMovementComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UAbilitySystemComponent;
class USDAttributeSet;
class FLifetimeProperty;

struct FInputActionValue;

UCLASS()
class SMALLDUNGEONS_API ASDCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASDCharacter(const FObjectInitializer& ObjectInitializer);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	USDCharacterMovementComponent* GetSDCharacterMovementComponent() const { return SDCharacterMovementComponent;};

	virtual void StopJumping() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:

	UFUNCTION(BlueprintCallable, Category= Movement)
	void Move(const FVector2D& Value);

	UFUNCTION(BlueprintCallable, Category= Movement)
	void Look(const FVector2D& Value);
	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = true))
	USDCharacterMovementComponent* SDCharacterMovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Abilities, meta = (AllowPrivateAccess = true))
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Abilities, meta = (AllowPrivateAccess = true))
	USDAttributeSet* AttributeSet;
};
