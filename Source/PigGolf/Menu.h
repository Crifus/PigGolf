// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu.generated.h"

/**
 * 
 */

class UButton;

UCLASS()
class PIGGOLF_API UMenu : public UUserWidget
{
	GENERATED_BODY()
	
private:

	// this meta link this variable to the button on the children WBP
	// name of the variable should be exactly the same of the button
	UPROPERTY(meta = (BindWidget))
		UButton* PlayGame;

	UPROPERTY(meta = (BindWidget))
		UButton* More;

	UPROPERTY(meta = (BindWidget))
		UButton* Less;

	UFUNCTION()
	void PlayGameClicked();
	UFUNCTION()
	void MoreClicked();
	UFUNCTION()
	void LessClicked();


public:

	UFUNCTION(BlueprintCallable)
	void Setup();

	UPROPERTY(BlueprintReadOnly)
	int32 Players{ 1 };

	virtual bool Initialize() override;
};
