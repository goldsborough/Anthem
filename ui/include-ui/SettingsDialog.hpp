#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:

    SettingsDialog(QWidget* parent = nullptr);

private:

    void setupUi();

    void saveSettings_();

};

#endif // SETTINGSDIALOG_HPP
