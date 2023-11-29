
#pragma once

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "webserv.hpp"

class Location {

public:
	Location(std::string const &block);
	~Location();

	//getters

	//setters

	//others

	class LocationError : public std::exception {
	
		public:
			// LocationError();
			// virtual ~LocationError();
			const char *what() const throw();
	};

private:
	Location();
	Location(Location const &copy);
	Location	&operator=(Location const &other);

	//params par block location {}
};

#endif
