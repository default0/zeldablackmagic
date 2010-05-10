#include <QtGui>
#include "mainwindow.h"
#include "preferences.h"
#include "canvas.h"

MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent)
{
    resize(640, 480);
    move(QApplication::desktop()->rect().width()/2-this->width()/2,
         QApplication::desktop()->rect().height()/2-this->height()/2);
    // Initialize the central widget here
    m_centralWidget = new QWidget;
    createActions();
    createMenus();
    setupToolbar();
    setupTabs();
    setupLayout();
    statusBar()->showMessage(tr("Welcome to Black Magic!"));
    //startRenderer();
    setWindowTitle(tr("Black Magic"));
    setCentralWidget(m_centralWidget);
}
MainWindow::~MainWindow()
{

}

void MainWindow::createActions()
{
    // File Menu
    m_actionOpen = new QAction(tr("&Open"), this);
    m_actionOpen->setStatusTip(tr("Open a Zelda3 ROM for editing..."));
    m_actionOpen->setShortcut(QKeySequence::Open);
    m_actionSave = new QAction(tr("&Save"), this);
    m_actionSaveAs = new QAction(tr("Save &As..."), this);
    m_actionExit = new QAction(tr("&Exit"), this);
    m_actionExit->setShortcut(QKeySequence::Quit);

    // File Menu connections (In order)
    connect(m_actionExit, SIGNAL(triggered()), this, SLOT(close()));

    // Edit Menu
    m_actionPreferences = new QAction(tr("&Preferences"), this);
    m_actionPreferences->setStatusTip(tr("View and Edit Black Magic Settings..."));

    // Edit Menu connections(In order)
    connect(m_actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));

}

void MainWindow::createMenus()
{

    // File Menu
    m_menuFile = menuBar()->addMenu(tr("&File"));
    m_menuFile->addAction(m_actionOpen);
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actionSave);
    m_menuFile->addAction(m_actionSaveAs);
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actionExit);

    // Edit Menu
    m_menuEdit = menuBar()->addMenu(tr("&Edit"));
    m_menuEdit->addSeparator();
    m_menuEdit->addSeparator();
    m_menuEdit->addAction(m_actionPreferences);
}

void MainWindow::setupToolbar()
{
}

void MainWindow::setupTabs()
{
    // The "Tools" tab widget
    m_tools = new QTabWidget;
    m_tools->resize(100, 0);
    m_tools->setMinimumWidth(100);
    m_tools->setMaximumWidth(300);
    m_tabTiles = new QWidget;
    QPushButton* pushAddTab = new QPushButton(m_tabTiles);
    pushAddTab->setText("Add Tab");
    connect(pushAddTab, SIGNAL(clicked()), this, SLOT(addTab()));
    m_tabRoomProperties = new QWidget;
    m_tools->addTab(m_tabTiles, tr("Tiles"));
    m_tools->addTab(m_tabRoomProperties, tr("Room Properties"));

    // The "Render" tab widget (This is just prelim to get the layout situated)
    m_render = new QTabWidget;
    m_render->setTabsClosable(true);

    for(int i = 0; i < 12; i++)
    {
        QWidget* widget = new QWidget;
        Canvas* sfmlCanvas = new Canvas(widget);
        QString title("Render");
        title.append(QString().number(i+1));
        widget->setObjectName("obj"+title);
        m_render->addTab(widget, title );
        m_genericWidget.append(widget);
        m_sfmlCanvas.append(sfmlCanvas);
    }
    connect(m_render, SIGNAL(tabCloseRequested(int)), this, SLOT(renderTabClose(int)));

    // The splitter
    m_mainSplitter = new QSplitter;
    m_mainSplitter->addWidget(m_tools);
    m_mainSplitter->addWidget(m_render);
}

void MainWindow::setupLayout()
{
    m_mainLayout = new QHBoxLayout;
    m_mainLayout->addWidget(m_mainSplitter);
    m_centralWidget->setLayout(m_mainLayout);
}


// Slots
void MainWindow::renderTabClose(int index)
{
    m_render->removeTab(index);

    delete m_sfmlCanvas[index];
    delete m_genericWidget[index];
    m_sfmlCanvas.removeAt(index);
    m_genericWidget.removeAt(index);
}

void MainWindow::addTab()
{
    QWidget* widget = new QWidget;
    Canvas* canvas = new Canvas(widget);

    QString title("Render");
    title.append(QString().number(m_render->count()+1));
    widget->setObjectName("obj"+title);
    m_sfmlCanvas.append(canvas);
    m_genericWidget.append(widget);

    m_render->addTab(widget, title );
}

void MainWindow::showPreferences()
{
    Preferences *pref = new Preferences(this);
    pref->setWindowFlags(Qt::SubWindow);
    pref->setWindowModality(Qt::ApplicationModal);
    pref->show();
}

