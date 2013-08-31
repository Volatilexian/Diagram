#include "diagramwindow.h"

DiagramWindow::DiagramWindow(QWidget *parent)
    : QMainWindow(parent)
{
    scene = new QGraphicsScene(0, 0, 600, 500);

    view = new QGraphicsView();
    view->setScene(scene);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    view->setContextMenuPolicy(Qt::ActionsContextMenu);
    setCentralWidget(view);

    minZ = 0;
    maxZ = 0;
    seqNumber = 0;

    createActions();
    createMenus();
    createToolBar();
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(updateActions()));

    setWindowTitle(tr("Diagram"));
    updateActions();
}

void DiagramWindow::createActions()
{
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    addNodeAction = new QAction(QIcon(":/images/node.png"), tr("Add &Node"), this);
    addNodeAction->setShortcut(QKeySequence::New);
    connect(addNodeAction, SIGNAL(triggered()), this, SLOT(addNode()));

    addLinkAction = new QAction(QIcon(":/images/link.png"), tr("Add &Link"), this);
    addLinkAction->setShortcut(tr("Ctrl+L"));
    connect(addLinkAction, SIGNAL(triggered()), this, SLOT(addLink()));

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Del"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(del()));

    cutAction = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    bringToFrontAction = new QAction(QIcon(":/images/bringtofront.png"), tr("Bring to &Front"), this);
    connect(bringToFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));

    sendToBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("&Send to Back"), this);
    connect(sendToBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    propertiesAction = new QAction(tr("P&roperties..."),this);
    connect(propertiesAction, SIGNAL(triggered()), this, SLOT(properties()));
}

void DiagramWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(addNodeAction);
    editMenu->addAction(addLinkAction);
    editMenu->addAction(deleteAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(bringToFrontAction);
    editMenu->addAction(sendToBackAction);
    editMenu->addSeparator();
    editMenu->addAction(propertiesAction);
}

void DiagramWindow::createToolBar()
{
    toolBar = addToolBar(tr("Edit"));
    toolBar->addAction(addNodeAction);
    toolBar->addAction(addLinkAction);
    toolBar->addAction(deleteAction);
    toolBar->addSeparator();
    toolBar->addAction(cutAction);
    toolBar->addAction(copyAction);
    toolBar->addAction(pasteAction);
    toolBar->addSeparator();
    toolBar->addAction(bringToFrontAction);
    toolBar->addAction(sendToBackAction);
}

