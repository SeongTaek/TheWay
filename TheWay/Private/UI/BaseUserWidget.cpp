// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseUserWidget.h"
#include "UI/InGameHUD.h"
#include "Controller/BasePlayerController.h"

void UBaseUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UBaseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBaseUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UBaseUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBaseUserWidget::Init()
{
}

ABasePlayerController* UBaseUserWidget::GetPlayerController()
{
	if (OwnerController == nullptr) 
	{
		OwnerController = Cast<ABasePlayerController>(GetOwningPlayer());
	}

	return OwnerController;
}

AInGameHUD* UBaseUserWidget::GetInGameHUD()
{
	return Cast<AInGameHUD>(GetPlayerController()->GetHUD());
}
