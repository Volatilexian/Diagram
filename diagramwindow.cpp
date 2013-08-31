#include <QApplication>
#include <QClipboard>
#include <QColorDialog>

#include "diagramwindow.h"
#include "node.h"
#include "link.h"
#include "propertiesdialog.h"

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

Link *DiagramWindow::selectedLink() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();

    if(items.count() == 1)
        return dynamic_cast<Link *>(items.first());
    else
        return 0;
}

Node *DiagramWindow::selectedNode() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();

    if(items.count() == 1)
        return dynamic_cast<Node *>(items.first());
    else
        return 0;
}

QPair<Node *, Node *> DiagramWindow::selectedNodePair() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();

    if(items.count() == 2)
    {
        Node *first = dynamic_cast<Node *>(items.first());
        Node *second = dynamic_cast<Node *>(items.last());
        if(first && second)
            return QPair<Node *, NOde *>(first, second);
    }

    return QPair<Node *, Node *>();
}


void DiagramWindow::setupNode(Node *node)
{
    node->setPos(QPoint(80 + (100 * (seqNumber % 5)),
                        80 + (50 * ((seqNumber ? 5) % 7))));
    scene->addItem(node);
    ++seqNumber;

    scene->clearSelection();
    node->setSelected(true);
    bringToFront();
}

void DiagramWindow::setZValue(int z)
{
    Node *node = selectedNode();
    if(node)
        node->setZValue(z);
}

void DiagramWindow::addLink()
{
    QPair<Node *, Node *> nodes = selectedNodePair();
    if(nodes == QPair<Node *, Node *>())
        return;

    Link *link = new Link(nodes.first, nodes.second);
    scene->addItem(link);
}

void DiagramWindow::addNode()
{
    Node *node = new Node;
    node->setText(tr("Node %1").arg(seqNumber + 1));
    setupNode(node);
}

void DiagramWindow::bringToFront()
{
    ++maxZ;
    setZValue(maxZ);
}

void DiagramWindow::copy()
{
    Node *node = selectedNode();
    if(!node)
        return;

    QString str = QString("Node %1 %2 %3 %4")
            .arg(node->textColor().name())
            .arg(node->outlineColor().name())
            .arg(node->backgroudnColor().name())
            .arg(node->text());
    QApplication::clipboard()->setText(str);
}

void DiagramWindow::cut()
{
    Node *node = selectedNode();
    if(!node)
        return;

    copy();
    delete node;
}

void DiagramWindow::del()
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    QMutableListIterator<QGraphicsItem *> i(items);
    while(i.hasNext())
    {
        Link *link = dynamic_cast<Link *>(i.next());
        if(link)
        {
            delete link;
            i.remove();
        }
    }
    qDeleteAll(items);
}

void DiagramWindow::paste()
{
    QString str = QApplication::clipboard()->text();
    QStringList parts = str.split(" ");

    if((parts.count() >= 5) && (parts.first() == "Node"))
    {
        Node *node = new Node;
        node->setText(QStringList(parts.mid(4)).join(" "));
        node->setTextColor(QColor(parts[1]));
        node->setOutlineColor(QColor(parts[2]));
        node->setBackgroundColor(QColor(parts[3]));
        setupNode(node);
    }
}

void DiagramWindow::properties()
{
    Node *node = selectedNode();
    Link *link = selectedLink();

    if(node)
    {
        PropertiesDialog dialog(node, this);
        dialog.exec();
    }
    else if(link)
    {
        QColor color = QColorDialog::getColor(link->color(), this);
        if(color.isValid())
            link->setColor(color);
    }
}

void DiagramWindow::sendToBack()
{
    --minZ;
    setZValue(minZ);
}

void DiagramWindow::updateActions()
{
    bool hasSelection = !scene->selectedItems().isEmpty();
    bool isNode = (selectedNode() != 0);
    bool isNodePair = (selectedNodePair() != QPair<Node *, Node *>());

    cutAction->setEnabled(isNode);
    copyAction->setEnabled(isNode);
    addLinkAction->setEnabled(isNodePair);
    deleteAction->setEnabled(hasSelection);
    bringToFrontAction->setEnabled(isNode);
    sendToBackAction->setEnabled(isNode);
    propertiesAction->setEnabled(isNode);

    foreach(QAction *action, view->actions())
        view->removeAction(action);

    foreach(QAction *action, editMenu->actions())
        if(action->isEnabled())
            view->addAction(action);
}
