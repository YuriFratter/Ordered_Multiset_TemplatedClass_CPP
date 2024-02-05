#include "elementNotFound.h"

elementNotFound::elementNotFound(const std::string &messaggioErrore) : std::runtime_error::runtime_error(messaggioErrore) {}