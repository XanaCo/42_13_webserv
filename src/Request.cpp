#include "../inc/Request.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Request(const std::string str)
{
    if (PRINT)
        std::cout << GREEN << "Request constructor called" << END_COLOR << std::endl;
}


Request::~Request()
{
    if (PRINT)
        std::cout << RED << "Response destructor called" << END_COLOR << std::endl;
}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

Request	&Request::operator=(Request const &obj)
{
    if (this != obj)
    {

    }
    return (*this);
}

std::ostream	&operator<<(std::ostream &os, Request &obj)
{
    os << REQUEST << " method:" <<obj._method << std::endl;
    return (os);
}

// ************************************************************************** //
//	METHODS
// ************************************************************************** //



// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

