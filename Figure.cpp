// Copyright of Wet Blanket Studios


#include "Figure.h"

#include <iostream>

#include "FigureDatum.h"
#include "FigureLexer.h"

UFigure::UFigure()
{
	// Stub
}

void UFigure::Read(const FString& Input)
{
	Data.Empty();

	// Initialize lexer
	Lexer = NewObject<UFigureLexer>(UFigureLexer::StaticClass());
	Lexer->Read(Input);

	// Initialize data
	SSIZE_T Position = 0;
	while (Position < Lexer->Tokens.Num())
	{
		// Position is incremented by FigureReadDatum
		Data.Add(FigureReadDatum(*Lexer, Position));
	}
}
