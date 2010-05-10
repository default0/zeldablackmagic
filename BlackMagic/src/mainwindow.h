#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

// Forward declarations
class QTabWidget;
class QHBoxLayout;
class QSplitter;
class QMenu;
class QAction;
class Canvas;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void renderTabClose(int);
    void showPreferences();
    void addTab();

private:
    void createActions();
    void createMenus();
    void setupToolbar();
    void setupTabs();
    void setupLayout();

    // File Menu
    QMenu* m_menuFile;
    QAction* m_actionOpen;
    QAction* m_actionSave;
    QAction* m_actionSaveAs;
    QAction* m_actionExit;
    QSplitter* m_mainSplitter;
    QWidget* m_centralWidget; // Dumb workaround to get the layout to show up
    QHBoxLayout* m_mainLayout;
    QTabWidget* m_tools;
    QTabWidget* m_render;


    // Edit Menu
    QMenu* m_menuEdit;
    QAction* m_actionPreferences;
    // Generic render tab for now (Simply to get it working)
    QList<QWidget *> m_genericWidget;
    QList<Canvas *> m_sfmlCanvas;
    // Tool Tabs
    QWidget* m_tabTiles;
    QWidget* m_tabRoomProperties;
};

#endif // MAINWINDOW_H
