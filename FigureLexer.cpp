// Copyright of Wet Blanket Studios


#include "Figure/FigureLexer.h"

#include "Misc/Char.h"

void UFigureLexer::Read(const FString& Input)
{
    Source = Input;

    SSIZE_T CurrentToken = 0;
    SSIZE_T Position = 0;
    auto Char = Source[Position];
    while (Char != '\0')
    {
        // Skip whitespace
        while (IsWhitespace(Char))
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
        if (Char == '\0')
        {
            return;
        }
        // Else, get token
        Tokens.AddDefaulted();

        // Check for identifier
        if (IsInitial(Char))
        {
            FString SrcStr;
            do
            {
                SrcStr += Char;
                ++Position;
                Char = Source[Position];
                if (!IsSubsequent(Char))
                {
                    break;
                }
            }
            while (Char != '\0');
            FFigureToken NewToken{ EFigureTokenType::Identifier, Position };
            NewToken.Value.Emplace<FString>(SrcStr);
            Tokens[CurrentToken] = NewToken;
        }
        else if (Char == '|')
        {
        }
        else
        {
            return;
        }
    }
}

bool UFigureLexer::IsDelim(TCHAR Char)
{
    if (IsWhitespace(Char) || (Char == '(') || (Char == ')') || (Char == '"')
        || (Char == '\'') || (Char == ';'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsWhitespace(TCHAR Char)
{
    if (TChar<TCHAR>::IsWhitespace(Char))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UFigureLexer::IsSpecial(TCHAR Char)
{
    switch (Char)
    {
    case '!':
    case '$':
    case '%':
    case '&':
    case '*':
    case '/':
    case ':':
    case '<':
    case '=':
    case '>':
    case '?':
    case '^':
    case '_':
    case '~':
        return true;
    default:
        return false;
    }
}

bool UFigureLexer::IsIdStart(TCHAR Char)
{
    return IsInitial(Char) || Char == '|' || IsPeculiar(Char);
}

bool UFigureLexer::IsExplicitSign(TCHAR Char)
{
    switch (Char)
    {
    case '+':
    case '-':
        return true;
    default:
        return false;
    }
}

bool UFigureLexer::IsPeculiar(TCHAR Char)
{
    return IsExplicitSign(Char) || Char == '.';
}

bool UFigureLexer::IsInitial(TCHAR Char)
{
    return TChar<TCHAR>::IsAlpha(Char) || IsSpecial(Char);
}

bool UFigureLexer::IsSubsequent(TCHAR Char)
{
    return IsInitial(Char) || TChar<TCHAR>::IsDigit(Char) || IsSpecialSubsequent(Char);
}

bool UFigureLexer::IsSpecialSubsequent(TCHAR Char)
{
    switch (Char)
    {
    case '+':
    case '-':
    case '.':
    case '@':
        return true;
    default:
        return false;
    }
}

bool UFigureLexer::IsSignSubsequent(TCHAR Char)
{
    return IsInitial(Char) || IsExplicitSign(Char) || Char == '@';
}

bool UFigureLexer::IsDotSubsequent(TCHAR Char)
{
    return IsSignSubsequent(Char) || Char == '.';
}

bool UFigureLexer::IsLineEnding(TCHAR Char)
{
    switch (Char)
    {
    case '\n':
    case '\r':
        return true;
    default:
        return false;
    }
}

bool UFigureLexer::IsNumber(TCHAR Char)
{
    return TChar<TCHAR>::IsDigit(Char);
}