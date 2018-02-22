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

#include "GuiApp/coreapplication.h"
#include <QSplashScreen>
#include <QDesktopWidget>

int CoreApplication::exec() {
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
