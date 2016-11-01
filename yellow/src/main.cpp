#include "AppInstance.h"

#include <string>
#include <vector>
#include <iostream>

#include "NetUtils.h"

int main(int argc, char *argv[]) {
    App::Instance app;

    return app.run(argc, argv);
}
