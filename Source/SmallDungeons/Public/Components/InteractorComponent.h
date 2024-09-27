// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interactable.h"
#include "InteractorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMALLDUNGEONS_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractorComponent();

	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool InvokeInteractOnQueueHead(UPARAM(meta = (Bitmask, BitmaskEnum = EItemType))const int32 DequeueableType);

	void QueueInteractable(const TScriptInterface<IInteractable>& Item);

	void DequeueInteractable(const TScriptInterface<IInteractable>& Item);
	
	UPROPERTY(BlueprintAssignable)
	FOnInteractableChanged OnInteractableChanged;
	
private:
	UPROPERTY(Transient)
	TArray<TScriptInterface<IInteractable>> InteractablesQueue;
};
