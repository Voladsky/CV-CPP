#include "OtsuApplication.h"

int main(int argc, char** argv) {
    OtsuApplication app;
    app.Initialize("Hatsune Miku Colorful Stage", 1280, 720);
    app.Run();

    return 0;
}
