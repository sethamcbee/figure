// Copyright of Wet Blanket Studios


#include "FigureDatum.h"

#include "FigureConsCell.h"
#include "FigureLexer.h"
#include "FigureString.h"
#include "FigureSymbol.h"

UFigureDatum* FigureReadDatum(UFigureLexer& Lexer, SSIZE_T& Position)
{
	const TArray<FFigureToken>& Tokens = Lexer.Tokens;

	const auto& Token = Tokens[Position];
	if (Token.Type == EFigureTokenType::String)
	{
		auto String = NewObject<UFigureString>(UFigureString::StaticClass());
		String->Read(Lexer, Position);
		return String;
	}
	else if (Token.Type == EFigureTokenType::Identifier)
	{
		auto Symbol = NewObject<UFigureSymbol>(UFigureSymbol::StaticClass());
		Symbol->Read(Lexer, Position);
		return Symbol;
	}
	else if (Token.Type == EFigureTokenType::LeftParenthesis)
	{
		// Recursively get elements of list
		auto List = NewObject<UFigureConsCell>(UFigureConsCell::StaticClass());
		List->Read(Lexer, Position);
		return List;
	}
	else
	{
		// TODO: Implement error datum
		auto Error = NewObject<UFigureDatum>(UFigureDatum::StaticClass());
		++Position;
		return Error;
	}
}

bool UFigureDatum::IsSimpleDatum()
{
	return false;
}

bool UFigureDatum::IsCompoundDatum()
{
	return false;
}

bool UFigureDatum::IsBool()
{
	return false;
}

bool UFigureDatum::IsNumber()
{
	return false;
}

bool UFigureDatum::IsCharacter()
{
	return false;
}

bool UFigureDatum::IsString()
{
	return false;
}

bool UFigureDatum::IsSymbol()
{
	return false;
}

bool UFigureDatum::IsList()
{
	return false;
}

bool UFigureDatum::IsAbbreviation()
{
	return false;
}

bool UFigureDatum::IsAbbrevPrefix()
{
	return false;
}
