/**
 * @file lambda.h
 */

#ifndef LAMBDA_H
#define LAMBDA_H

/// Defines a lambda.
class Lambda
{
public:

    ~Lambda();

    std::vector<std::string> args;
    Env env;
    std::vector<Exp*> bodies;
};

#endif // LAMBDA_H
