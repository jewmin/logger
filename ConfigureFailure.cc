#include "ConfigureFailure.h"

namespace Logger {

ConfigureFailure::ConfigureFailure(const std::string & reason) : std::runtime_error(reason) {
}

}