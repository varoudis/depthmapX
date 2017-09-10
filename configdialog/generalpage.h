#pragma once

#include <QWidget>
#include "settingspage.h"
#include <iostream>

class GeneralPage : public SettingsPage
{
private:
    bool m_simpleVersion = false;    
    void readSettings(Settings &settings) {
        m_simpleVersion = settings.readSetting(SettingTag::simpleVersion, true).toBool();
    }
public:
    GeneralPage(Settings &settings, QWidget *parent = 0);
    virtual void writeSettings(Settings &settings) override {
        settings.writeSetting(SettingTag::simpleVersion, m_simpleVersion);
    }
};
