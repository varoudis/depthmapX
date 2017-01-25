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
