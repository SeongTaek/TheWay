// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NoticeWaveWidget.h"
#include "Components/TextBlock.h"

void UNoticeWaveWidget::SetWave(int32 Wave)
{
	static FText WaveTextFormat = FText::FromString(TEXT("Wave {0}"));

	Txt_Wave->SetText(FText::Format(WaveTextFormat, Wave));
}
