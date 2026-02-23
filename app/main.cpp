#include <engine/core/Application.h>
#include <cstdio>

int main(int argc, char *argv[])
{
    fprintf(stderr, "=== 2DNGE starting ===\n");
    Application app;
    fprintf(stderr, "=== Application created, entering run() ===\n");
    app.run();
    fprintf(stderr, "=== run() returned ===\n");

    return 0;
}