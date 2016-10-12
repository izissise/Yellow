#include "AppInstance.h"

int main(int argc, char *argv[]) {
    App::Instance app;

    return app.run(argc, argv);
}
