#include "Projectbar.hpp"
#include "IconButton.hpp"
#include "CustomMessageBox.hpp"
#include "PopupLine.hpp"
#include "CustomMenu.hpp"

#include <QtWidgets>
#include <QtCore>
#include <QMenu>

#include <QHBoxLayout>

Projectbar::Projectbar(QWidget* parent)
    : QWidget(parent), modified_(false),
      dir_(new QDir(QDir::homePath(),"*.anthem"))
{
    if (! dir_->cd("Documents"))
    { qWarning("Could not change directory"); }

    projects_ = dir_->entryInfoList();

    currProject_ = projects_.begin();

    setupUi();
}

Projectbar::~Projectbar()
{
    delete dir_;
}

void Projectbar::setupUi()
{
    QHBoxLayout* layout = new QHBoxLayout(this);

    layout->setMargin(0);

    layout->setSpacing(0);

    layout->setContentsMargins(0,0,0,0);

    IconButton* previousButton = new IconButton(QIcon(":/icons/previous.png"),
                                                QIcon(":/icons/previous-active.png"),
                                                this);

    previousButton->setObjectName("PreviousButton");

    layout->addWidget(previousButton);

    connect(previousButton, &QPushButton::clicked,
            this, &Projectbar::previousProject);

    IconButton* nextButton = new IconButton(QIcon(":/icons/next.png"),
                                            QIcon(":/icons/next-active.png"),
                                            this);

    nextButton->setObjectName("NextButton");

    layout->addWidget(nextButton);

    connect(nextButton, &QPushButton::clicked,
            this, &Projectbar::nextProject);


    projectLabel_ = new QLabel(currProject_->baseName(), this);

    layout->addWidget(projectLabel_);


    IconButton* menuButton = new IconButton(QIcon(":/icons/menu.png"),
                                            QIcon(":/icons/menu-active.png"),
                                            this);

    menuButton->setObjectName("MenuButton");

    layout->addWidget(menuButton);



    CustomMenu* menu = new CustomMenu(menuButton, this);

    /* --------- Refresh Action -------- */

    QAction* refreshAction = new QAction("Refresh directory",this);

    refreshAction->setShortcut(tr("Ctrl + R + D"));

    connect(refreshAction, &QAction::triggered,
            this, &Projectbar::refreshDirectory);

    menu->addAction(refreshAction);


    /* --------- Change Action -------- */

    QAction* changeAction = new QAction("Change directory",this);

    changeAction->setShortcut(tr("Ctrl + C + D"));

    connect(changeAction, &QAction::triggered,
            this, &Projectbar::changeDirectory);

    menu->addAction(changeAction);


    /* --------- Open Action -------- */

    QAction* openAction = new QAction("Open project", this);

    openAction->setShortcut(tr("Ctrl + O"));

    connect(openAction, &QAction::triggered,
            this, &Projectbar::openProject);

    menu->addAction(openAction);

    /* --------- New Action -------- */

    QAction* newAction = new QAction("&New project", this);

    newAction->setShortcut(tr("Ctrl + N"));

    connect(newAction, &QAction::triggered,
            this, &Projectbar::newProject);

    menu->addAction(newAction);


    /* --------- Save Action -------- */

    QAction* saveAction = new QAction("Save", this);

    saveAction->setShortcut(tr("Ctrl + S"));

    connect(saveAction, &QAction::triggered,
            this, &Projectbar::save);

    menu->addAction(saveAction);

    /* --------- Save As Action -------- */

    QAction* saveAsAction = new QAction("Save As", this);

    saveAsAction->setShortcut(tr("Ctrl + Shift + S"));

    // Does the same thing, as of now
    connect(saveAsAction, &QAction::triggered,
            newAction, &QAction::trigger);

    menu->addAction(saveAsAction);

    /* --------- This Window -------- */

    QWidget::setLayout(layout);
}

void Projectbar::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Projectbar::setModified(bool boolean)
{
    modified_ = boolean;
}

bool Projectbar::isModified() const
{
    return modified_;
}

void Projectbar::nextProject()
{
    if (currProject_ + 1 < projects_.end())
    {
        projectLabel_->setText((++currProject_)->baseName());
        // read file ...
    }
}

void Projectbar::previousProject()
{
    if (currProject_ != projects_.begin())
    {
        projectLabel_->setText((--currProject_)->baseName());
        // read file ...
    }
}

void Projectbar::refreshDirectory()
{
    dir_->refresh();
}

void Projectbar::changeDirectory()
{
    QString path = QFileDialog::getExistingDirectory(this,
                                                     "Change directory",
                                                     dir_->absolutePath(),
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    if(! path.isEmpty())
    {
        dir_->cd(path);

        if(dir_->entryInfoList().isEmpty())
        {
            CustomMessageBox message("Invalid directory",
                                     "The directory you chose does not\n contain any Anthem project files!",
                                     this
                                     );

            QPushButton* createnewProjectButton = new QPushButton("Create new project in this directory", &message);

            message.addButton(createnewProjectButton);

            connect(createnewProjectButton, &QPushButton::clicked,
                    this, &Projectbar::newProject);

            QPushButton* chooseDifferentButton = new QPushButton("Choose different directory", &message);

            message.addButton(chooseDifferentButton);

            connect(chooseDifferentButton, &QPushButton::clicked,
                    this, &Projectbar::changeDirectory);

            QPushButton* stayInCurrentButton = new QPushButton("Stay in current directory", &message);

            message.addButton(stayInCurrentButton);

            connect(chooseDifferentButton, &QPushButton::clicked,
                    &message, &CustomMessageBox::close);

            message.exec();

        }

        else
        {
            projects_ = dir_->entryInfoList();

            currProject_ = projects_.begin();

            projectLabel_->setText(currProject_->baseName());
        }
    }
}

void Projectbar::openProject()
{
    QFileInfo file = QFileDialog::getOpenFileName(this,
                                                  "Open File",
                                                  dir_->absolutePath(),
                                                  "*.anthem");
    if(file.isFile())
    {
        dir_->cd(file.path());

        projects_ = dir_->entryInfoList();

        currProject_ = projects_.begin();

        // Find correct iterator position for file
        while(*currProject_ != file) ++currProject_;

        projectLabel_->setText(file.baseName());
    }
}

void Projectbar::newProject()
{
    IconButton* icon = new IconButton(QIcon(":/icons/folder.png"),
                                      QIcon(":/icons/folder-active.png"),
                                      this);

    connect(icon, &IconButton::clicked,
            this, &Projectbar::changeDirectory);

    PopupLine popup(this, icon);

    QString fileName;

    connect(&popup, &PopupLine::receivedInput,
            [&] (const QString& text) { fileName = text; });

    popup.exec();

    if (fileName.isEmpty()) return;

    QStringList path = fileName.split(".");

    // If the user added the extension
    if (path.size() > 1)
    {
        // Set to correct extension
        if (path[1] != "anthem") path[1] = "anthem";

        fileName = path.join(".");
    }

    else fileName = path[0] + ".anthem";

    if (dir_->exists(fileName))
    {
        CustomMessageBox message("Project already exists",
                                 "A project with that name already\n exists in the current directory!",
                                 this
                                 );

        QPushButton* overwriteButton = new QPushButton("Overwrite", this);

        message.addButton(overwriteButton);

        connect(overwriteButton, &QPushButton::clicked,
                &message, &CustomMessageBox::close);

        QPushButton* tryAgainButton = new QPushButton("Try again",this);

        message.addButton(tryAgainButton);

        message.exec();

        if (message.lastButtonPressed() == tryAgainButton)
        {
            message.close();
            newProject();
            return;
        }
    }

    projectLabel_->setText(path[0]);

    // Make fileName absolute
    fileName = dir_->path() + "/" + fileName;

    // create and save file
    QFile file(fileName);

    file.open(QFile::WriteOnly);

    file.write("ramen");

    file.close();

    dir_->refresh();

    projects_ = dir_->entryInfoList();

    currProject_ = projects_.begin();

    // Find correct iterator position for file
    while(*currProject_ != fileName) ++currProject_;
}

void Projectbar::save()
{
    if (modified_)
    {
        // Save file

        modified_ = false;
    }
}
