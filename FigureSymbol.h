// Copyright of Wet Blanket Studios

#pragma once

#include "CoreMinimal.h"
#include "FigureDatum.h"
#include "FigureSymbol.generated.h"

/**
 * 
 */
UCLASS()
class PAPERTOWNS_API UFigureSymbol : public UFigureDatum
{
	GENERATED_BODY()
	
public:

	void Read(UFigureLexer& Lexer, SSIZE_T& Position);

	virtual bool IsSimpleDatum();
	virtual bool IsSymbol();

	UPROPERTY(EditAnywhere)
	FString Value;
};
