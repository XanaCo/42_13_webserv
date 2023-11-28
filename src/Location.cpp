
#include "../inc/Location.hpp"

/*::: CONSTRUCTORS :::*/

Location::Location(std::string const &block) {

	/// set params 
	(void)block;
	if (PRINT)
		std::cout << GREEN << "Constructor: Location created " << END_COLOR << std::endl;

	return ;
}

/*::: DESTRUCTORS :::*/

Location::~Location() {

	if (PRINT)
		std::cout << RED << "Destructor: Location destroyed " << END_COLOR << std::endl;

	return ;
}

/*::: Operator Overloading ::: */

std::ostream &operator<<(std::ostream &out, Location const &other) {
	
	(void)other;
	out << "all LocationInfos here to debug";

	return out;
}

/*::: ACCESSORS :::*/

// all geters here

/*::: MEMBER FUNCTIONS :::*/

// other functions here

/*::: EXCEPTIONS :::*/

const char *Location::LocationError::what() const throw() {

	return YELLOW "Webserv Error : Location" END_COLOR;
}
