// Copyright of Wet Blanket Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Figure.generated.h"

/**
 * 
 */
UCLASS()
class PAPERTOWNS_API UFigure : public UObject
{
	GENERATED_BODY()

public:

	UFigure();

	UFUNCTION(BlueprintCallable)
	void Read(const FString& Input);
};
