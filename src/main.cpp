#include "LumaApplication.h"

int main(int argc, char** argv) {
    LumaApplication app;
    app.Initialize("Hatsune Miku Colorful Stage", 1280, 720);
    app.Run();

    return 0;
}
