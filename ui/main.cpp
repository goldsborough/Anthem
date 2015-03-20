#include "AnthemUi.hpp"

#include <QApplication>
#include <QFile>
#include <QString>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile file("/Users/petergoldsborough/Documents/Anthem/ui/rsc-ui/style/style.css");

	file.open(QFile::ReadOnly);

	app.setStyleSheet(QString(file.readAll()));

	AnthemUi anthemUi;

	anthemUi.show();

    return app.exec();
}
