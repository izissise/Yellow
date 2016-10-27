#ifndef SnifferObserver_hpp
#define SnifferObserver_hpp

#include <stdio.h>

namespace Net {
    class SnifferObserver {
    public:
        virtual void update() = 0;
    };
}

#endif /* SnifferObserver_hpp */
