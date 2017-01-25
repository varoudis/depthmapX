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
