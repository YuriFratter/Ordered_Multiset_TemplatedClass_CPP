#ifndef ELEMENTNOTFOUND_H
#define ELEMENTNOTFOUND_H
#include <stdexcept>		//std::runtime_error
#include <string>			//std::string

/**
 * @brief Eccezione custom elementNotFound
 * Eccezione che eredita da std::runtime_error, viene lanciata quando si tenta di rimuovere 
 * un elemento che non è presente nel multiset.
 */
class elementNotFound : public std::runtime_error {
	public:
		elementNotFound(const std::string &messaggioErrore);
};

#endif