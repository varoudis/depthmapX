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

#ifndef MAINWINDOWFACTORY_H
#define MAINWINDOWFACTORY_H

#include <QMainWindow>
#include <QDialog>

class MainWindowHolder
{
public:
    MainWindowHolder();
    ~MainWindowHolder();
    QMainWindow& get();

private:
    QMainWindow* m_window;
    MainWindowHolder(const MainWindowHolder& );
    MainWindowHolder& operator=(const MainWindowHolder& );
};

class LicenseAgreementHolder
{
public:
    LicenseAgreementHolder();
    ~LicenseAgreementHolder();
    QDialog& get();
private:
    QDialog* m_licenseDialog;
    LicenseAgreementHolder(const LicenseAgreementHolder& );
    LicenseAgreementHolder& operator=(const LicenseAgreementHolder&);
};

#endif // MAINWINDOWFACTORY_H
