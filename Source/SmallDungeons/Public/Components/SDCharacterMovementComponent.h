// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SDCharacterMovementComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMALLDUNGEONS_API USDCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USDCharacterMovementComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void DoJumpWithGravity();

	virtual void StopJumpWithGravity();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	UFUNCTION()
	void JumpTimelineStep();

	UFUNCTION()
	void JumpTimelineFinish();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Jumping", meta = (AllowPrivateAccess = true))
	float DefaultGravityValue;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Jumping", meta = (AllowPrivateAccess = true))
	UCurveFloat* JumpGravityCurve;
	
	FTimeline JumpTimeline;
};
