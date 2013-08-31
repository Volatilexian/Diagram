#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>

class Node;
class Link;

class DiagramWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    DiagramWindow(QWidget *parent = 0);

private slots:
    void addNode();
    void addLink();
    void del();
    void cut();
    void copy();
    void paste();
    void bringToFront();
    void sendToBack();
    void properties();
    void updateActions();

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void setZValue(int z);
    void setupNode(Node *node);
    Node *selectedNode() const;
    Link *selectedLink() const;
    QPair<Node *, Node *> selectedNodePair() const;

    QMenu *fileMenu;
    QMenu *editMenu;
    QToolBar *toolBar;
    QAction *exitAction;
    QAction *addNodeAction;
    QAction *addLinkAction;
    QAction *deleteAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *bringToFrontAction;
    QAction *sendToBackAction;
    QAction *propertiesAction;

    QGraphicsScene *scene;
    QGraphicsView *view;

    int minZ;
    int maxZ;
    int seqNumber;
};

#endif // DIAGRAMWINDOW_H
