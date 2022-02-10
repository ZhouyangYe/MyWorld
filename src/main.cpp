#include "Core/App.h"

int main(void)
{
    MyWorld::App::Init();

    MyWorld::App::Run();

    MyWorld::App::Terminate();

    return 0;
}
