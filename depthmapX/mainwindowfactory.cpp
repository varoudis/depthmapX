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

#include "mainwindowfactory.h"
#include "mainwindow.h"
#include "licenseagreement.h"

MainWindowHolder::MainWindowHolder()
{
    m_window = dynamic_cast<QMainWindow*>(new MainWindow());
}

MainWindowHolder::~MainWindowHolder()
{
    delete m_window;
}

QMainWindow& MainWindowHolder::get()
{
    return *m_window;
}


LicenseAgreementHolder::LicenseAgreementHolder()
{
    m_licenseDialog = dynamic_cast<QDialog*>(new LicenseAgreement());
}

LicenseAgreementHolder::~LicenseAgreementHolder()
{
    delete m_licenseDialog;
}

QDialog& LicenseAgreementHolder::get()
{
    return *m_licenseDialog;
}
