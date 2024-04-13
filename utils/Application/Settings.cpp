#include "Settings.hpp"

Settings::Settings(){}

Settings* Settings::get() {
    if (!settings) {settings = new Settings();}
    return settings;
}

void Settings::reset() {
    if (settings) {delete settings;}
}

Settings::~Settings(){delete settings;}