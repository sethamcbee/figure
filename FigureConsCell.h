// Copyright of Wet Blanket Studios

#pragma once

#include "CoreMinimal.h"
#include "Figure/FigureDatum.h"
#include "FigureConsCell.generated.h"

/**
 * 
 */
UCLASS()
class PAPERTOWNS_API UFigureConsCell : public UFigureDatum
{
	GENERATED_BODY()

public:

	void Read(UFigureLexer& Lexer, SSIZE_T& Position);

	virtual bool IsCompoundDatum();
	virtual bool IsList();

	UPROPERTY(EditAnywhere)
	UFigureDatum* Car = nullptr;
	
	UPROPERTY(EditAnywhere)
	UFigureDatum* Cdr = nullptr;
};
