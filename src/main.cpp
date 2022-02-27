#include "Core/App.h"

int main(void)
{
    MyWorld::App::Init();

    while (!MyWorld::Window::shouldWindowTerminate())
    {
        MyWorld::App::Run();
    }

    MyWorld::App::Terminate();

    return 0;
}
