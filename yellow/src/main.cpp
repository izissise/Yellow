#include "AppInstance.h"

#include <string>

int main(int argc, char *argv[]) {
    App::Instance app;
    app.run(argc, argv);

    return 0;
}
