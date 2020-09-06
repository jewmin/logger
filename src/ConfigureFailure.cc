#include "ConfigureFailure.h"

namespace Logger {

ConfigureFailure::ConfigureFailure(const i8 * reason) : Common::CExpection(reason) {
}

ConfigureFailure::~ConfigureFailure() {
}

}