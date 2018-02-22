// Copyright (C) 2017 Petros Koutsolampros

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "depthmapX/mainwindowfactory.h"
#include "version.h"
#include "settingsimpl.h"
#include <QApplication>
#include <QFileOpenEvent>
#include <QtDebug>

class CoreApplication : public QApplication
{
private:
    QString mFileToLoad;
    std::unique_ptr<MainWindow> mMainWindow;
public:
    CoreApplication(int &argc, char **argv)
        : QApplication(argc, argv)
    {
    }

    bool event(QEvent *event)
    {
        // this event is triggered in macOS, either by calling "Open with..."
        // in Finder, or by dropping a file on the depthmapX icon on the dock
        // more info: http://doc.qt.io/qt-5/qfileopenevent.html
        if (event->type() == QEvent::FileOpen) {
            QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
            mFileToLoad = openEvent->file();
            mMainWindow->loadFile(openEvent->file());
        }

        return QApplication::event(event);
    }

    int exec() {
        SettingsImpl settings(new DefaultSettingsFactory);

        if (!settings.readSetting(SettingTag::licenseAccepted, false).toBool())
        {
            auto dummy = MainWindowFactory::getLicenseDialog();
            dummy->setModal(true);
            dummy->setWindowTitle(TITLE_BASE);
            dummy->exec();
            if ( dummy->result() == QDialog::Rejected) {
                return 0;
            }
            settings.writeSetting(SettingTag::licenseAccepted, true);
        }

        QSplashScreen *splash = 0;
        int screenId = QApplication::desktop()->screenNumber();
        splash = new QSplashScreen(QPixmap(QLatin1String("images/splash.png")));
        if (QApplication::desktop()->isVirtualDesktop())
        {
            QRect srect(0, 0, splash->width(), splash->height());
            splash->move(QApplication::desktop()->availableGeometry(screenId).center() - srect.center() );
        }

        auto args = arguments();
        QString fileToLoad = mFileToLoad;
        if (args.length() == 2)
        {
            fileToLoad = args[1];
        }

        mMainWindow = MainWindowFactory::getMainWindow(fileToLoad, settings);
        mMainWindow->show();
        return QApplication::exec();
    }
};
