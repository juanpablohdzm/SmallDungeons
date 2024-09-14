// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SDCharacterMovementComponent.h"


// Sets default values for this component's properties
USDCharacterMovementComponent::USDCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USDCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void USDCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USDCharacterMovementComponent::DoJumpWithGravity()
{
	FOnTimelineFloat OnTimelineStep;
	FOnTimelineEvent OnTimelineEnd;
	OnTimelineStep.BindUFunction(this, FName("JumpTimelineStep"));
	OnTimelineEnd.BindUFunction(this, FName("JumpTimelineEnd"));
	
	JumpTimeline.AddInterpFloat(JumpGravityCurve, MoveTemp(OnTimelineStep));
	JumpTimeline.SetTimelineFinishedFunc(MoveTemp(OnTimelineEnd));

	JumpTimeline.Play();
}

void USDCharacterMovementComponent::StopJumpWithGravity()
{
	JumpTimeline.Stop();
	JumpTimelineFinish();
}

void USDCharacterMovementComponent::JumpTimelineStep()
{
	UE_LOG(LogTemp, Warning, TEXT("JP: Jumptimeline step"));
	const float PlaybackPosition = JumpTimeline.GetPlaybackPosition();
	GravityScale = JumpGravityCurve->GetFloatValue(PlaybackPosition);
}

void USDCharacterMovementComponent::JumpTimelineFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("JP: Jumptimeline finish"));
	GravityScale = DefaultGravityValue;
}
