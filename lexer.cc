/**
 * @file lexer.cc
 */

#include "lexer.h"

namespace Figure
{

bool is_delim(char c)
{
    if (isspace(c) || (c == '(') || (c == ')') || (c == '"')
		|| (c == '\'') || (c == ';'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_special(Char c)
{
	switch (c)
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

bool is_id_start(Char c)
{
	return is_initial(c) || c == '|';
}

bool is_initial(Char c)
{
	return isalpha(c) || is_special(c);
}

bool is_subsequent(Char c)
{
	return is_initial(c) || isdigit(c) || is_special_subsequent(c);
}

bool is_special_subsequent(Char c)
{
	switch (c)
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

bool is_line_ending(Char c)
{
	switch (c)
	{
	case '\n':
	case '\r':
		return true;
	default:
		return false;
	}
}

Lexer::Lexer(std::istream& s)
{
	input = &s;
	input->get(ch);
}

void Lexer::next_token()
{
	std::string src;

	while (input->good() && (isspace(ch) || ch == ';'))
	{
		// Skip whitespace.
		if (isspace(ch))
		{
			src += ch;
			while (input->good())
			{
				input->get(ch);
				if (!isspace(ch))
				{
					break;
				}
			}
			source += src;
			pos += src.length();
			src = "";
		}

		// Skip comments.
		if (ch == ';')
		{
			src += ch;
			input->get(ch);
			while (input->good() && !is_line_ending(ch))
			{
				src += ch;
				input->get(ch);
			}
			source += src;
			pos += src.length();
			src = "";
		}
	}

	// EOF.
	if (!input->good())
	{
		Token tok{LEOF{}, pos};
		tokens.push_back(tok);
	}
	// Identifier.
	else if (is_initial(ch))
	{
		src += ch;
		while (input->good())
		{
			input->get(ch);
			if (!is_subsequent(ch))
			{
				break;
			}
			src += ch;
		}
		Token tok{Id{src}, pos};
		tokens.push_back(tok);
	}
	else if (ch == '|')
	{
		src += ch;
		while (input->good())
		{
			input->get(ch);
			src += ch;
			if (ch == '|')
			{
				input->get(ch);
				break;
			}
		}
		Token tok{Id{src}, pos};
		tokens.push_back(tok);
	}
	// Booleans and characters.
	else if (ch == '#')
	{
		src += ch;
		if (input->good())
		{
			input->get(ch);
			if (ch == 'f')
			{
				src += ch;
				Token tok{Bool{false}, pos};
				tokens.push_back(tok);
				input->get(ch);
			}
			else if (ch == 't')
			{
				src += ch;
				Token tok{Bool{true}, pos};
				tokens.push_back(tok);
				input->get(ch);
			}
			else
			{
				error("Invalid character.");
			}
		}
		else
		{
			error("Invalid character.");
		}
	}
	// Strings.
	else if (ch == '"')
	{
		source += ch;
		++pos;
		bool done = false;
		while (input->good())
		{
			input->get(ch);
			if (ch == '"')
			{
				done = true;
				break;
			}
			src += ch;
		}
		if (!done)
		{
			error("Mismatched quote.");
		}
		Token tok{String{src}, pos - 1};
		tokens.push_back(tok);
		source += src;
		source += ch;
		input->get(ch);
		pos += src.length() + 1;
		return;
	}
	// Parenthesis.
	else if (ch == '(')
	{
		src += ch;
		input->get(ch);
		Token tok{LeftParen{}, pos};
		tokens.push_back(tok);
	}
	else if (ch == ')')
	{
		src += ch;
		input->get(ch);
		Token tok{RightParen{}, pos};
		tokens.push_back(tok);
	}
	// Quote.
	else if (ch == '\'')
	{
		src += ch;
		input->get(ch);
		Token tok{Quote{}, pos};
		tokens.push_back(tok);
	}
	else if (ch == '`')
	{
		src += ch;
		input->get(ch);
		Token tok{Backtick{}, pos};
		tokens.push_back(tok);
	}
	else if (ch == ',')
	{
		src += ch;
		input->get(ch);
		if (input->good() && ch == '@')
		{
			src += ch;
			input->get(ch);
			Token tok{CommaAt{}, pos};
			tokens.push_back(tok);
		}
		else
		{
			Token tok{Comma{}, pos};
			tokens.push_back(tok);
		}
	}
	else
	{
		error("Invalid character.");
	}
	
	source += src;
	pos += src.length();
}

Token Lexer::get_token()
{
	// Check if tokens are left.
	if (tokens.empty())
	{
		next_token();
	}
	auto tok = tokens.front();
	tokens.pop_front();
	return tok;
}

void Lexer::push_token(const Token& tok)
{
	tokens.push_front(tok);
}

bool Lexer::good()
{
	if (!input->good())
	{
		return false;
	}

	// Check if we are at EOF.
	if (tokens.empty())
	{
		next_token();
	}
	auto tok = tokens.front();
	if (std::get_if<LEOF>(&tok.value))
	{
		return false;
	}
	else
	{
		return true;
	}
}

}
