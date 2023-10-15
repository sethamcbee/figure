// Copyright of Wet Blanket Studios

#pragma once

#include "FigureDatum.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FigureString.generated.h"

/**
 * 
 */
UCLASS()
class PAPERTOWNS_API UFigureString : public UFigureDatum
{
	GENERATED_BODY()
	
public:

	void Read(UFigureLexer& Lexer, SSIZE_T& Position);

	virtual bool IsSimpleDatum();
	virtual bool IsString();

	UPROPERTY(EditAnywhere)
	FString Value;
};
