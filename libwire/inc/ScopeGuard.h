#ifndef _SCOPE_GUARD_H_
#define _SCOPE_GUARD_H_

#include <functional>

class ScopeGuard
{
public:
    ScopeGuard(std::function<void()> callback);
    ~ScopeGuard();

    void deactivate() { _deactivated = true; }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
private:
    bool _deactivated = false;
    std::function<void()> _callback;
};

#endif

