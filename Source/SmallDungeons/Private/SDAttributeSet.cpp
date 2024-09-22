// Fill out your copyright notice in the Description page of Project Settings.


#include "SDAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

USDAttributeSet::USDAttributeSet()
{
	CurrentHealth.SetBaseValue(100.0f);
	CurrentHealth.SetCurrentValue(100.0f);

	MaxHealth.SetBaseValue(100.0f);
	MaxHealth.SetCurrentValue(100.0f);
}

void USDAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USDAttributeSet, CurrentHealth, OldCurrentHealth);
}

void USDAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USDAttributeSet, MaxHealth, OldMaxHealth);
}

void USDAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetCurrentHealthAttribute())
	{
		const float MaxHealthValue = MaxHealth.GetCurrentValue();
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxHealthValue);
	}
}

void USDAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		float NewHealth = FMath::Clamp(CurrentHealth.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue());
		SetCurrentHealth(NewHealth);
	}
}

void USDAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USDAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USDAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}