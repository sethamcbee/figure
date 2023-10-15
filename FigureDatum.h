// Copyright of Wet Blanket Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FigureDatum.generated.h"

class UFigureLexer;

UFigureDatum* FigureReadDatum(UFigureLexer& Lexer, SSIZE_T& Position);

/**
 * 
 */
UCLASS()
class PAPERTOWNS_API UFigureDatum : public UObject
{
	GENERATED_BODY()
	
public:

	virtual bool IsSimpleDatum();
	virtual bool IsCompoundDatum();
	virtual bool IsBool();
	virtual bool IsNumber();
	virtual bool IsCharacter();
	virtual bool IsString();
	virtual bool IsSymbol();
	virtual bool IsList();
	virtual bool IsAbbreviation();
	virtual bool IsAbbrevPrefix();

	UPROPERTY(EditAnywhere)
	UFigureLexer* SourceLexer = nullptr;
	
	SSIZE_T SourcePosition = 0;
};
