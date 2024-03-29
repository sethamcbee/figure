// Copyright of Wet Blanket Studios


#include "Figure/FigureLexer.h"

#include "Misc/Char.h"

void UFigureLexer::Read(const FString& Input)
{
    Source = Input;

    SSIZE_T CurrentToken = 0;
    SSIZE_T Position = 0;
    TCHAR Char = Source[Position];
    while (Position < Source.Len())
    {
        // Skip whitespace
        while (IsWhitespace(Position))
        {
            ++Position;
            Char = Source[Position];
        }

        // Skip comments
        if (Char == ';')
        {
            while (Char != '\n' && Char != '\0')
            {
                ++Position;
                Char = Source[Position];
            }

            // Start at beginning and check for more whitespace
            continue;
        }

        // Check for end of source
        if (Position >= Source.Len())
        {
            return;
        }

        // Else, get token
        if (IsIdentifierStart(Position))
        {
            Tokens.Add(GetIdentifier(Position));
        }
        else if (IsBoolStart(Position))
        {
            Tokens.Add(GetBool(Position));
        }
        else if (IsNumberStart(Position))
        {
            Tokens.Add(GetNumber(Position));
        }
        else if (IsCharacterStart(Position))
        {
            Tokens.Add(GetCharacter(Position));
        }
        else if (IsStringStart(Position))
        {
            Tokens.Add(GetString(Position));
        }
        else if (Char == '(')
        {
            Tokens.Add(FFigureToken{ EFigureTokenType::LeftParenthesis, Position });
            ++Position;
        }
        else if (Char == '(')
        {
            Tokens.Add(FFigureToken{ EFigureTokenType::RightParenthesis, Position });
            ++Position;
        }
        else if (Char == '\'')
        {
            Tokens.Add(FFigureToken{ EFigureTokenType::Apostrophe, Position });
            ++Position;
        }
        else if (Char == '`')
        {
            Tokens.Add(FFigureToken{ EFigureTokenType::Backtick, Position });
            ++Position;
        }
        else if (Char == ',')
        {
            Tokens.Add(FFigureToken{ EFigureTokenType::Comma, Position });
            ++Position;
        }
        else if (Char == '.')
        {
            Tokens.Add(FFigureToken{ EFigureTokenType::Period, Position });
            ++Position;
        }
        else
        {
            Error("Invalid token");
            ++Position;
        }
    }
}

void UFigureLexer::Error(const FString& Msg)
{
    UE_LOG(LogTemp, Warning, TEXT("FigureLexer error: %s"), *Msg);
}

FFigureToken UFigureLexer::GetIdentifier(SSIZE_T& Position)
{
    SSIZE_T StartingPosition = Position;
    TCHAR Char = Source[Position];
    if (IsInitial(Position))
    {
        FString SrcStr{ "" };
        SrcStr += Char;
        ++Position;
        while (IsSubsequent(Position))
        {
            SrcStr += Source[Position];
            ++Position;
        }
        FFigureToken NewToken{ EFigureTokenType::Identifier, StartingPosition };
        NewToken.Value.Emplace<FString>(SrcStr);
        return NewToken;
    }
    else if (IsVerticalLine(Position))
    {
        FString SrcStr{ "" };
        SrcStr += Char;
        ++Position;
        while (IsSymbolElement(Position))
        {
            SrcStr += Source[Position];
            ++Position;
        }
        if (IsVerticalLine(Position))
        {
            SrcStr += '|';
            FFigureToken NewToken{ EFigureTokenType::Identifier, StartingPosition };
            NewToken.Value.Emplace<FString>(SrcStr);
            return NewToken;
        }
        else
        {
            Error("No matching vertical line for identifier");
            FFigureToken NewToken{ EFigureTokenType::Error, StartingPosition };
            return NewToken;
        }
    }
    else
    {
        Error("Could not parse identifier");
        FFigureToken NewToken{ EFigureTokenType::Error, StartingPosition };
        return NewToken;
    }
}

bool UFigureLexer::IsIdentifierStart(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (IsInitial(Position))
    {
        return true;
    }
    else if (IsVerticalLine(Position))
    {
        // TODO: Check if this has matching vertical line
        return true;
    }
    else if (IsPeculiarStart(Position))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsInitial(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (TChar<TCHAR>::IsAlpha(Char))
    {
        return true;
    }
    else if (IsSpecialInitial(Position))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsSpecialInitial(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (Char == '!')
    {
        return true;
    }
    else if (Char == '$')
    {
        return true;
    }
    else if (Char == '%')
    {
        return true;
    }
    else if (Char == '&')
    {
        return true;
    }
    else if (Char == '*')
    {
        return true;
    }
    else if (Char == '/')
    {
        return true;
    }
    else if (Char == ':')
    {
        return true;
    }
    else if (Char == '<')
    {
        return true;
    }
    else if (Char == '=')
    {
        return true;
    }
    else if (Char == '>')
    {
        return true;
    }
    else if (Char == '?')
    {
        return true;
    }
    else if (Char == '^')
    {
        return true;
    }
    else if (Char == '_')
    {
        return true;
    }
    else if (Char == '~')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsSubsequent(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (IsInitial(Position))
    {
        return true;
    }
    else if (TChar<TCHAR>::IsDigit(Char))
    {
        return true;
    }
    else if (IsSpecialSubsequent(Position))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsSpecialSubsequent(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (IsExplicitSign(Position))
    {
        return true;
    }
    else if (Char == '.' || Char == '@')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsVerticalLine(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (Char == '|')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsLineEnding(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (Char == '\n' || Char == '\r')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsComment(SSIZE_T Position)
{
    // TODO: Implement comments
    return false;
}

bool UFigureLexer::IsNestedComment(SSIZE_T Position)
{
    return false;
}

bool UFigureLexer::IsCommentText(SSIZE_T Position)
{
    return false;
}

bool UFigureLexer::IsCommentCont(SSIZE_T Position)
{
    return false;
}

bool UFigureLexer::IsAtmosphere(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (IsWhitespace(Position))
    {
        return true;
    }
    else if (IsComment(Position))
    {
        return true;
    }
    else
    {
        // TODO: Implement directives
        return false;
    }
}

bool UFigureLexer::IsIntertokenSpace(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (IsAtmosphere(Position))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsSymbolElement(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (IsVerticalLine(Position))
    {
        return false;
    }
    else if (Char == '\\')
    {
        return false;
    }
    else
    {
        // TODO: Implement inline hex escapes, mnenomic escapes, and '\|'
        return true;
    }
}

FFigureToken UFigureLexer::GetPeculiar(SSIZE_T Position)
{
    return FFigureToken();
}

bool UFigureLexer::IsPeculiarStart(SSIZE_T Position)
{
    // TODO: Implement +i, -i, and NaN
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (IsExplicitSign(Position))
    {
        return true;
    }
    else if (Char == '.')
    {
        // TODO: Check for valid dot subsequents
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsExplicitSign(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (Char == '+' || Char == '-')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsSignSubsequent(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (IsInitial(Position))
    {
        return true;
    }
    else if (IsExplicitSign(Position))
    {
        return true;
    }
    else if (Char == '@')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsDotSubsequent(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (IsSignSubsequent(Position))
    {
        return true;
    }
    else if (Char == '.')
    {
        return true;
    }
    else
    {
        return false;
    }
}

FFigureToken UFigureLexer::GetBool(SSIZE_T& Position)
{
    return FFigureToken();
}

bool UFigureLexer::IsBoolStart(SSIZE_T Position)
{
    // TODO: Implement bool
    return false;
}

FFigureToken UFigureLexer::GetNumber(SSIZE_T& Position)
{
    return FFigureToken();
}

bool UFigureLexer::IsNumberStart(SSIZE_T Position)
{
    // TODO: Implement numbers
    return false;
}

FFigureToken UFigureLexer::GetCharacter(SSIZE_T& Position)
{
    return FFigureToken();
}

bool UFigureLexer::IsCharacterStart(SSIZE_T Position)
{
    // TODO: Implement characters
    return false;
}

bool UFigureLexer::IsCharacterName(SSIZE_T Position)
{
    return false;
}

bool UFigureLexer::IsHexScalarValue(SSIZE_T Position)
{
    return false;
}

FFigureToken UFigureLexer::GetString(SSIZE_T& Position)
{
    auto StartingPosition = Position;
    // Skip beginning quotation
    FString SrcStr = TEXT("");
    ++Position;
    while (IsStringElement(Position))
    {
        SrcStr += Source[Position];
        ++Position;
    }
    // Check for ending quotation
    if (Source[Position] == '\"')
    {
        ++Position;
        FFigureToken NewToken{ EFigureTokenType::String, StartingPosition };
        NewToken.Value.Emplace<FString>(SrcStr);
        return NewToken;
    }
    else
    {
        Error("Unterminated string");
        FFigureToken NewToken{ EFigureTokenType::Error, StartingPosition };
        return NewToken;
    }
}

bool UFigureLexer::IsStringStart(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (Char == '"')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsStringElement(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (Char == '"')
    {
        return false;
    }
    else if (Char == '\\')
    {
        // TODO: Implement escapes
        return false;
    }
    else if (IsMnemonicEscape(Position))
    {
        return true;
    }
    else
    {
        return true;
    }
}

bool UFigureLexer::IsMnemonicEscape(SSIZE_T Position)
{
    return false;
}

bool UFigureLexer::IsDelimiter(SSIZE_T Position)
{
    return false;
}

bool UFigureLexer::IsIntralineSpace(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (Char == ' ')
    {
        return true;
    }
    else if (Char == '\t')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsWhitespace(SSIZE_T Position)
{
    if (Position >= Source.Len())
    {
        return false;
    }

    TCHAR Char = Source[Position];
    if (IsIntralineSpace(Position))
    {
        return true;
    }
    else if (IsLineEnding(Position))
    {
        return true;
    }
    else
    {
        return false;
    }
}
