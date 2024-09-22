// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SDAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class SMALLDUNGEONS_API USDAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
    USDAttributeSet();

    UPROPERTY(BlueprintReadOnly, Category= "Health", ReplicatedUsing=OnRep_CurrentHealth)
    FGameplayAttributeData CurrentHealth;
    ATTRIBUTE_ACCESSORS(USDAttributeSet, CurrentHealth)

    UPROPERTY(BlueprintReadOnly, Category= "Health", ReplicatedUsing=OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(USDAttributeSet, MaxHealth)

protected:
    UFUNCTION()
    virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth);
    UFUNCTION()
    virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
