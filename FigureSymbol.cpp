// Copyright of Wet Blanket Studios


#include "FigureSymbol.h"

#include "FigureLexer.h"

void UFigureSymbol::Read(UFigureLexer& Lexer, SSIZE_T& Position)
{
	SourceLexer = &Lexer;
	SourcePosition = Position;
	auto& Tokens = Lexer.Tokens;
	auto& Token = Tokens[Position];
	if (Token.Type == EFigureTokenType::Identifier)
	{
		Value = Token.Value.Get<FString>();
		++Position;
	}
	else
	{
		// Error
		Value = TEXT("ERROR");
		++Position;
	}
}

bool UFigureSymbol::IsSimpleDatum()
{
	return true;
}

bool UFigureSymbol::IsSymbol()
{
	return true;
}
