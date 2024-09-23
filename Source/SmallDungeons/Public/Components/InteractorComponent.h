// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interactor.h"
#include "InteractorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableChanged)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMALLDUNGEONS_API UInteractorComponent : public UActorComponent, public IInteractor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractorComponent();

	UFUNCTION(BlueprintCallable)
	void Interact();

	void SetInteractable_Implementation(UObject* Object) override;

	UObject* GetInteractable_Implementation() override;
private:
	UPROPERTY(Transient)
	TQueue<UObject*> InteractablesQueue;
};
