// Copyright of Wet Blanket Studios

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EFigureTokenType : uint8
{
	None,
	Error,
	Identifier,
	Boolean,
	Number,
	Character,
	String,
	LeftParenthesis,
	RightParenthesis,
	Apostrophe,
	Backtick,
	Comma,
	CommaAt,
	Period
};

struct FFigureToken
{
	using ValueType = TVariant<FString, char, bool, float>;

	EFigureTokenType Type = EFigureTokenType::None;
	SIZE_T Position = 0;
	ValueType Value;
};
