// Copyright of Wet Blanket Studios

#pragma once

#include <string>

#include "Figure/FigureToken.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FigureLexer.generated.h"

/**
 * Lexer for Figure scripts and data
 */
UCLASS()
class PAPERTOWNS_API UFigureLexer : public UObject
{
	GENERATED_BODY()
	
public:

	void Read(const FString& Input);

    FString Source;
	TArray<FFigureToken> Tokens;

private:

    // Token helper functions
    bool IsDelim(TCHAR Char);
    bool IsWhitespace(TCHAR Char);
    bool IsSpecial(TCHAR Char);
    bool IsIdStart(TCHAR Char);
    bool IsExplicitSign(TCHAR Char);
    bool IsPeculiar(TCHAR Char);
    bool IsInitial(TCHAR Char);
    bool IsSubsequent(TCHAR Char);
    bool IsSpecialSubsequent(TCHAR Char);
    bool IsSignSubsequent(TCHAR Char);
    bool IsDotSubsequent(TCHAR Char);
    bool IsLineEnding(TCHAR Char);
    bool IsNumber(TCHAR Char);
};
