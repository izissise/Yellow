#include "ScopeGuard.h"

ScopeGuard::ScopeGuard(std::function<void()> callback)
: _callback(callback) {
}

ScopeGuard::~ScopeGuard() {
    if (!_deactivated)
        _callback();
}
