#include <QFrame>
#include <QTimerEvent>
#include "TraceManager.h"
#include "BrowseDialog.h"
#include "StringUtil.h"
#include "MiscUtil.h"

TraceManager::TraceManager(QWidget* parent) : QTabWidget(parent)
{
    setMovable(true);

    //Open
    mOpen = new QPushButton(this);
    mOpen->setIcon(DIcon("folder-horizontal-open")); //TODO: New icon
    mOpen->setToolTip(tr("Open"));
    connect(mOpen, SIGNAL(clicked()), this, SLOT(open()));
    setCornerWidget(mOpen, Qt::TopRightCorner);

    //Close All Tabs
    mCloseAllTabs = new QPushButton(this);
    mCloseAllTabs->setIcon(DIcon("close-all-tabs"));
    mCloseAllTabs->setToolTip(tr("Close All Tabs"));
    connect(mCloseAllTabs, SIGNAL(clicked()), this, SLOT(closeAllTabs()));
    setCornerWidget(mCloseAllTabs, Qt::TopLeftCorner);

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(Bridge::getBridge(), SIGNAL(openTraceFile(const QString &)), this, SLOT(openSlot(const QString &)));

    // Add a placeholder tab
    QFrame* mPlaceholder = new QFrame(this);
    addTab(mPlaceholder, tr("Placeholder")); //TODO: This is only to prevent open buttons from disappearing
}

TraceManager::~TraceManager()
{
    closeAllTabs();
}

void TraceManager::open()
{
    BrowseDialog browse(
        this,
        tr("Open trace recording"),
        tr("Trace recording"),
        tr("Trace recordings (*.%1);;All files (*.*)").arg(ArchValue("trace32", "trace64")),
        getDbPath(),
        false
    );
    if(browse.exec() != QDialog::Accepted)
        return;
    openSlot(browse.path);
}

void TraceManager::openSlot(const QString & path)
{
    //load the new file
    TraceWidget* newView = new TraceWidget(Bridge::getArchitecture(), path, this);
    addTab(newView, path); //TODO: Proper title
    int index = count() - 1;
    setCurrentIndex(index);
    connect(newView, &TraceWidget::closeFile, this, [index, this]()
    {
        closeTab(index);
    });
}

void TraceManager::closeTab(int index)
{
    auto view = qobject_cast<TraceWidget*>(widget(index));
    if(view)
    {
        removeTab(index);
        mViewsToDelete.append(view); // It needs to return from close event before we can delete
        startTimer(100);
    }
    else
    {
        // Placeholder tab
        return;
    }
}

void TraceManager::closeAllTabs()
{
    bool closeBack = true;
    while(count() > 1)
    {
        if(closeBack)
        {
            int beforeTabs = count();
            closeTab(count() - 1);
            if(count() == beforeTabs) // Placeholder tab can't be closed, so close tabs before it instead
                closeBack = false;
        }
        else
        {
            closeTab(0);
        }
    }
}

// These tabs are deleted after the close tab event completes
void TraceManager::timerEvent(QTimerEvent* event)
{
    for(auto & i : mViewsToDelete)
        delete i;
    mViewsToDelete.clear();
    killTimer(event->timerId());
}
