#ifndef FILEBAR_HPP
#define FILEBAR_HPP

#include <QWidget>
#include <QFileInfoList>
#include <QString>

class QLabel;
class QPaintEvent;
class QDir;
class PopupLine;

class Projectbar : public QWidget
{
    Q_OBJECT

public:

    Projectbar(QWidget *parent = 0);

    ~Projectbar();

    void setModified(bool boolean = true);

    bool isModified() const;

    void createNewProject(QString fileName);

public slots:

    void nextProject();

    void previousProject();

    void refreshDirectory();

    void changeDirectory();

    void openProject();

    void newProject();

    void save();

private:

    void paintEvent(QPaintEvent* event);

    void setupUi();

    bool modified_;

    PopupLine* newProjectPopupLine_;

    QLabel* projectLabel_;

    QDir* dir_;

    QFileInfoList projects_;

    QFileInfoList::iterator currProject_;

};

#endif // FILEBAR_HPP
