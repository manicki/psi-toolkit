#include "configurator.hpp"

#include "config.h"

Configurator& Configurator::getInstance() {
    static Configurator instance;

    return instance;
}

bool Configurator::isRunAsInstalled() const {
    return runAsInstalled_;
}

void Configurator::setRunAsInstalled(bool state) {
    runAsInstalled_ = state;
}

Configurator::Configurator():runAsInstalled_(
#if IS_INSTALLABLE
true
#else
false
#endif
) {
}
