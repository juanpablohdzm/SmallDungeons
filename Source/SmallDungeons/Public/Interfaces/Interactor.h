﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactor.generated.h"

class IInteractable;
// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UInteractor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMALLDUNGEONS_API IInteractor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void QueueInteractable(const TScriptInterface<IInteractable>& Item);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DequeueInteractable(const TScriptInterface<IInteractable>& Item);
};
