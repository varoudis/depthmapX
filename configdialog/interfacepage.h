#pragma once

#include <QWidget>
#include <QtWidgets>
#include "settingspage.h"
#include <map>
#include <memory>

class InterfacePage : public SettingsPage
{
    Q_OBJECT

private:
    QColor m_background;
    QColor m_foreground;
    std::map<QListWidgetItem *, QColor *> colourMap;
    void readSettings(Settings &settings) {
        m_foreground = QColor(settings.readSetting(SettingTag::foregroundColour, qRgb(128,255,128)).toInt());
        m_background = QColor(settings.readSetting(SettingTag::backgroundColour, qRgb(0,0,0)).toInt());
    }
private slots:
    void onInterfaceColourlItemClicked(QListWidgetItem *item);
public:
    InterfacePage(Settings &settings, QWidget *parent = 0);
    virtual void writeSettings(Settings &settings) override {
        settings.writeSetting(SettingTag::backgroundColour, m_background.rgb());
        settings.writeSetting(SettingTag::foregroundColour, m_foreground.rgb());
    }
};
