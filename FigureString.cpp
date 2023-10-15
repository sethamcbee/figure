// Copyright of Wet Blanket Studios


#include "FigureString.h"

#include "FigureLexer.h"

void UFigureString::Read(UFigureLexer& Lexer, SSIZE_T& Position)
{
	SourceLexer = &Lexer;
	SourcePosition = Position;
	auto& Tokens = Lexer.Tokens;
	auto& Token = Tokens[Position];
	if (Token.Type == EFigureTokenType::String)
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

bool UFigureString::IsSimpleDatum()
{
	return true;
}

bool UFigureString::IsString()
{
	return true;
}
