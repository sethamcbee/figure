// Copyright of Wet Blanket Studios


#include "Figure/FigureConsCell.h"

#include "Figure/FigureLexer.h"

void UFigureConsCell::Read(UFigureLexer& Lexer, SSIZE_T& Position)
{
	SourceLexer = &Lexer;
	SourcePosition = Position;
	auto& Tokens = Lexer.Tokens;

	// Skip left parenthesis
	++Position;

	// Find matching right parenthesis
	int Parenthesis = 1;
	SSIZE_T ListEnd = Position;
	while (ListEnd < Lexer.Tokens.Num())
	{
		auto& Token = Lexer.Tokens[ListEnd];
	}
}

bool UFigureConsCell::IsCompoundDatum()
{
	return true;
}

bool UFigureConsCell::IsList()
{
	return true;
}
