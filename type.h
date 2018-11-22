/**
 * @file type.h
 */

#ifndef TYPE_H
#define TYPE_H

/// Defines a fundamental data type identifier.
enum class Type
{
    VOID,
    NUMBER,
    BOOLEAN,
    SYMBOL,
    STRING,
    LAMBDA,
    LIST,
    NATIVE_FUNCTION
};

typedef double Number_Type;

#endif // TYPE_H
