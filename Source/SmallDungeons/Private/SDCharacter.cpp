// Fill out your copyright notice in the Description page of Project Settings.


#include "SDCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SDCharacterMovementComponent.h"

// Sets default values
ASDCharacter::ASDCharacter(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<USDCharacterMovementComponent>(CharacterMovementComponentName))
{
	SDCharacterMovementComponent = Cast<USDCharacterMovementComponent>(GetCharacterMovement());
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Orientation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Spring arm component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	FollowCamera->SetupAttachment(SpringArmComponent);

	// Enhanced Jump
	FOnTimelineFloat OnTimelineStep;
	FOnTimelineEvent OnTimelineEnd;
	OnTimelineStep.BindUFunction(this, FName("JumpTimelineStep"));
	OnTimelineEnd.BindUFunction(this, FName("JumpTimelineEnd"));
	
	JumpTimeline.AddInterpFloat(JumpGravityCurve, MoveTemp(OnTimelineStep));
	JumpTimeline.SetTimelineFinishedFunc(MoveTemp(OnTimelineEnd));

}

// Called when the game starts or when spawned
void ASDCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASDCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tick the jump timeline
	JumpTimeline.TickTimeline(DeltaTime);

}

// Called to bind functionality to input
void ASDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASDCharacter::Move(const FVector2D& Value)
{
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Value.Y);
		AddMovementInput(RightDirection, Value.X);
	}
}

void ASDCharacter::Look(const FVector2D& Value)
{
	if (Controller)
	{
		AddControllerPitchInput(Value.Y);
		AddControllerYawInput(Value.X);
	}
}

void ASDCharacter::Jump()
{
	Super::Jump();
	JumpTimeline.PlayFromStart();
}

void ASDCharacter::StopJumping()
{
	Super::StopJumping();
	FinishJump();
}

void ASDCharacter::ResetJumpState()
{
	Super::ResetJumpState();

	JumpTimeline.Stop();
	GetSDCharacterMovementComponent()->GravityScale = 1.0f;
}

void ASDCharacter::JumpTimelineStep()
{
	const float PlaybackPosition = JumpTimeline.GetPlaybackPosition();
	GetSDCharacterMovementComponent()->GravityScale = JumpGravityCurve->GetFloatValue(PlaybackPosition);
}

void ASDCharacter::FinishJump()
{
	const float FinalPosition = JumpTimeline.GetTimelineLength();
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, FString::Format(TEXT("Jump timeline finish final position {0}"),{FinalPosition}));
	GetSDCharacterMovementComponent()->GravityScale = JumpGravityCurve->GetFloatValue(FinalPosition);
}

