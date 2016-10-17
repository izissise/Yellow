#ifndef _SCOPE_GUARD_H_
#define _SCOPE_GUARD_H_

#include <functional>

class ScopeGuard
{
public:
    ScopeGuard(std::function<void()> callback);
    ~ScopeGuard();

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
private:
    std::function<void()> _callback;
};

#endif

