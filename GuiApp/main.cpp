// Copyright (C) 2011-2012, Tasos Varoudis
// Copyright (C) 2017 Christian Sailer

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

#include <QPixmap>
#include <QDir>
#include <QSplashScreen>
#include <QDesktopWidget>
#include <QDateTime>

#include "mainwindowfactory.h"
#include "coreapplication.h"
#include "version.h"

#ifdef _WIN32
#include <windows.h>
#endif

//////// dX Simple //
// Search for #ifndef _COMPILE_dX_SIMPLE_VERSION in order to force "simple dX" compile


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resource);


    CoreApplication app(argc, argv);

    LicenseAgreementHolder dummy;
    dummy.get().setModal(true);
    dummy.get().setWindowTitle(TITLE_BASE);
    dummy.get().exec();
    if ( dummy.get().result() == dummy.get().Rejected ) return 0;

	QSplashScreen *splash = 0;
    int screenId = QApplication::desktop()->screenNumber();
    splash = new QSplashScreen(QPixmap(QLatin1String("images/splash.png")));
    if (QApplication::desktop()->isVirtualDesktop()) 
	{
        QRect srect(0, 0, splash->width(), splash->height());
        splash->move(QApplication::desktop()->availableGeometry(screenId).center() - srect.center() );
    }
    //splash->show();

    auto args = app.arguments();
    QString fileToLoad = "";
    if(app.fileToLoad.length() > 0) {
        fileToLoad = app.fileToLoad;
    }
    else if (args.length() == 2)
    {
        fileToLoad = args[1];
    }

    MainWindowHolder mainWindow(fileToLoad);
    mainWindow.get().show();

    //splash->finish(&mainWin);
    return app.exec();
}
