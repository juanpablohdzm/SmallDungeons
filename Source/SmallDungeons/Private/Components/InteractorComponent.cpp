// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractorComponent.h"

#include "Interfaces/Interactable.h"


UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInteractorComponent::InvokeInteractOnQueueHead(const int32 DequeueableType)
{
	if (InteractablesQueue.IsEmpty())
	{
		return false;
	}

	const TScriptInterface<IInteractable> Interactable = InteractablesQueue[0];
	const int32 ItemType = static_cast<int32>(IInteractable::Execute_Interact(Interactable.GetObject(), GetOwner()));
	if ((DequeueableType & ItemType) == ItemType)
	{
		DequeueInteractable(Interactable);
	}

	return true;
}

void UInteractorComponent::QueueInteractable(const TScriptInterface<IInteractable>& Item)
{
	if (InteractablesQueue.Contains(Item))
	{
		return;
	}

	InteractablesQueue.Add(Item);
	OnInteractableChanged.Broadcast();
}

void UInteractorComponent::DequeueInteractable(const TScriptInterface<IInteractable>& Item)
{
	InteractablesQueue.Remove(Item);
	OnInteractableChanged.Broadcast();
}

