// Copyright (C) 2011-2012, Tasos Varoudis

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


#include <QApplication>
#include <QPixmap>
#include <QDir>
#include <QSplashScreen>
#include <QDesktopWidget>
#include <QDateTime>

#include "mainwindow.h"
#include "licenseagreement.h"

#ifdef _WIN32
#include <windows.h>
#endif

//////// dX Simple //
// Search for #ifndef _COMPILE_dX_SIMPLE_VERSION in order to force "simple dX" compile


//pqmap<QString, QString> m_dll_load_errors;
//int m_next_module_id;
//pvector<CImportedModule> m_imported_modules;
//int m_next_imported_menu_id;

/*
int ImportModule(QString& path)
{
#ifdef _WIN32_XXX // testing
   path.replace(QString("/"), QString("\\"));
   CImportedModule module;
   module.m_path = path;

   module.m_inst = LoadLibrary((LPCWSTR)module.m_path.constData());

   if (module.m_inst == NULL) {
      m_dll_load_errors.add(path, QString("Failed to load DLL module"));
      return -1;
   }

   FUNC_GETMODULENAME getModuleName = (FUNC_GETMODULENAME)GetProcAddress(module.m_inst,"getModuleName");

   if (getModuleName == NULL) {
      // this may well be true when other DLLs are in the same folder
      // so, don't complain
      // it does mean that if someone forgets module name, they'll
      // never see a bug, so you should reinstate for SDK testing version
      // m_dll_load_errors.add(path,_T("Could not retrieve module name.\nPlease check that this DLL is a depthmapX plug-in module"));
      return -1;
   }

   module.m_name = getModuleName();

   FUNC_GETMODULEVERSION getModuleVersion = (FUNC_GETMODULEVERSION)GetProcAddress(module.m_inst,"getModuleVersion");

   if (getModuleVersion == NULL) {
      m_dll_load_errors.add(path, QString("Could not retrieve version information from module.\nPlease check that this DLL is a depthmapX plug-in module."));
      return -1;
   }
   else {
      float x= getModuleVersion();
      float y= float(DEPTHMAP_MODULE_VERSION);
      float z= float(DEPTHMAP_VERSION);
      if (x > z) {
         QString version;
         version = QString("The %1 module requires at least depthmapX version %2 in order to run.\nPlease update your copy of depthmapX.").arg(module.m_name).arg(getModuleVersion());
         m_dll_load_errors.add(path,version);
         return -1;
      }
      else if (x < y) {
         QString version;
         version = QString("The %1 module is incompatible with depthmapX version %2 and above.\nPlease ask your module provider to supply a more recent version.").arg(module.m_name).arg(y);
         m_dll_load_errors.add(path,version);
         return -1;
      }
   }

   FUNC_GETANALYSISNAME getAnalysisName = (FUNC_GETANALYSISNAME)GetProcAddress(module.m_inst,"getAnalysisName");
   FUNC_GETANALYSISTYPE getAnalysisType = (FUNC_GETANALYSISTYPE)GetProcAddress(module.m_inst,"getAnalysisType");

   int count = 0;
   if (getAnalysisName != NULL) {
      for (int i = 0; i < 8; i++) {
         if (getAnalysisName(i) != NULL) {
            module.m_analysis_name[i] = getAnalysisName(i);
            if (getAnalysisType != NULL) {
               module.m_analysis_type[i] = getAnalysisType(i);            }
            else {
               module.m_analysis_type[i] = DLL_NO_ANALYSIS;
            }
            count++;
         }
         else {
            break;
         }
      }
   }

   if (getAnalysisName == NULL || count == 0) {
      m_dll_load_errors.add(path, QString("Could not retrieve analysis name(s).\nPlease check that this DLL is a depthmapX plug-in module"));
      return -1;
   }
   module.count = count;
   m_imported_modules.push_back(module);
#endif
   return 0;
}
*/

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resource);

    QApplication app(argc, argv);

    LicenseAgreement dummy;
    dummy.setModal(true);
	dummy.setWindowTitle("depthmapX 0.25beta");
    dummy.exec();
    if ( dummy.result() == dummy.Rejected ) return 0;

	QSplashScreen *splash = 0;
    int screenId = QApplication::desktop()->screenNumber();
    splash = new QSplashScreen(QPixmap(QLatin1String("images/splash.png")));
    if (QApplication::desktop()->isVirtualDesktop()) 
	{
        QRect srect(0, 0, splash->width(), splash->height());
        splash->move(QApplication::desktop()->availableGeometry(screenId).center() - srect.center() );
    }
    //splash->show();

	QDateTime wait;
	int end, start = wait.secsTo(QDateTime::currentDateTime());
	end = 0;

    /*
///// Import Modules
	QString curDir = QDir::currentPath();
	QDir dir(curDir);
	QStringList filters;
    filters << "*.dll";
	QStringList files = dir.entryList(filters);
    for(int i=0; i<files.count(); i++)
    {
        QString str(curDir+ "/"+ files[i]);
        ImportModule(str);
    }
/////
*/

    //MainWindow mainWin(&m_imported_modules);
    MainWindow mainWin;

    while(end < start+2) end = wait.secsTo(QDateTime::currentDateTime());

    mainWin.show();

    //splash->finish(&mainWin);
    return app.exec();
}
