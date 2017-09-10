#pragma once
#include <QDialog>
#include "depthmapX/settings.h"
#include <vector>
#include <memory>
#include "configdialog/settingspage.h"

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigDialog(Settings &settings);

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void createIcons();
    std::vector<std::unique_ptr<SettingsPage>> settingsPages;
    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;
    Settings &m_settings;
    void saveChanges();
    void saveChangesAndClose();
};
