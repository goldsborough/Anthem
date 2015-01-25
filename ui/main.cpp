#include "mainwindow.hpp"
#include <QApplication>
#include <QFile>
#include <QString>

extern void qt_set_sequence_auto_mnemonic(bool b);

int main(int argc, char *argv[])
{
    qt_set_sequence_auto_mnemonic(true);

    QApplication app(argc, argv);

    QFile file("/Users/petergoldsborough/Documents/Anthem/ui/rsc-ui/style/style.css");
    file.open(QFile::ReadOnly);

    QString styleSheet = QString(file.readAll());
    app.setStyleSheet(styleSheet);

    MainWindow window;

    window.show();

    return app.exec();
}
