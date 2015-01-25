#ifndef FILEBAR_HPP
#define FILEBAR_HPP

#include <QWidget>
#include <QFileInfoList>

class QLabel;
class QPaintEvent;
class QMenu;
class QDir;

class Projectbar : public QWidget
{
    Q_OBJECT

public:

    Projectbar(QWidget *parent = 0);

    ~Projectbar();

    void paintEvent(QPaintEvent* event);

    void setModified(bool boolean = true);

    bool isModified() const;

public slots:

    void nextProject();

    void previousProject();

    void refreshDirectory();

    void changeDirectory();

    void openProject();

    void newProject();

    void save();

private:

    void setupUi();

    bool modified_;

    QLabel* projectLabel_;

    QMenu* menu_;

    QDir* dir_;

    QFileInfoList projects_;

    QFileInfoList::iterator currProject_;

};

#endif // FILEBAR_HPP
