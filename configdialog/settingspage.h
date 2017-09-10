#pragma once

#include "depthmapX/settings.h"

class SettingsPage : public QWidget
{
public:
    SettingsPage(Settings settings, QWidget *parent = 0) : QWidget(parent) {}
    virtual void writeSettings(Settings &settings) = 0;
};
