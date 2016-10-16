//
//  SnifferObserver.hpp
//  Yellow
//
//  Created by Remi Robert on 16/10/2016.
//
//

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
