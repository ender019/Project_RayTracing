#include "Application.hpp"

Settings* Settings::settings = nullptr;
Settings* sett = Settings::get();

int main() 
{
    App window;
    window.run();

    return 0;
}