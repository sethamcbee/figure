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

    void Error(const FString& Msg);

    // Token helper functions
    FFigureToken GetIdentifier(SSIZE_T& Position);
    bool IsIdentifierStart(SSIZE_T Position);
    bool IsInitial(SSIZE_T Position);
    bool IsSpecialInitial(SSIZE_T Position);
    bool IsSubsequent(SSIZE_T Position);
    bool IsSpecialSubsequent(SSIZE_T Position);
    bool IsVerticalLine(SSIZE_T Position);
    bool IsSymbolElement(SSIZE_T Position);
    FFigureToken GetPeculiar(SSIZE_T Position);
    bool IsPeculiarStart(SSIZE_T Position);
    bool IsExplicitSign(SSIZE_T Position);
    bool IsSignSubsequent(SSIZE_T Position);
    bool IsDotSubsequent(SSIZE_T Position);
    FFigureToken GetBool(SSIZE_T& Position);
    bool IsBoolStart(SSIZE_T Position);
    FFigureToken GetNumber(SSIZE_T& Position);
    bool IsNumberStart(SSIZE_T Position);
    FFigureToken GetCharacter(SSIZE_T& Position);
    bool IsCharacterStart(SSIZE_T Position);
    bool IsCharacterName(SSIZE_T Position);
    bool IsHexScalarValue(SSIZE_T Position);
    FFigureToken GetString(SSIZE_T& Position);
    bool IsStringStart(SSIZE_T Position);
    bool IsStringElement(SSIZE_T Position);
    bool IsMnemonicEscape(SSIZE_T Position);
    bool IsDelimiter(SSIZE_T Position);
    bool IsIntralineSpace(SSIZE_T Position);
    bool IsWhitespace(SSIZE_T Position);
    bool IsLineEnding(SSIZE_T Position);
    bool IsComment(SSIZE_T Position);
    bool IsNestedComment(SSIZE_T Position);
    // TODO: Datum comments
    bool IsCommentText(SSIZE_T Position);
    bool IsCommentCont(SSIZE_T Position);
    // TODO: Directive
    bool IsAtmosphere(SSIZE_T Position);
    bool IsIntertokenSpace(SSIZE_T Position);
};
