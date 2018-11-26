/**
 * @file lambda.h
 */

#ifndef LAMBDA_H
#define LAMBDA_H

/// Defines a lambda.
class Lambda
{
public:

    std::vector<std::string> args;
    std::shared_ptr<Env> env;
    std::vector<std::shared_ptr<Exp>> bodies;
};

#endif // LAMBDA_H
