#include <QFrame>
#include "TraceManager.h"
#include "BrowseDialog.h"
#include "StringUtil.h"
#include "MiscUtil.h"

TraceManager::TraceManager(QWidget* parent) : QTabWidget(parent)
{
    setMovable(true);
    setTabsClosable(true);

    //Open
    mOpen = new QPushButton(this);
    mOpen->setIcon(DIcon("control-record")); //TODO: New icon
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

    // Add a placeholder tab
    QFrame* mPlaceholder = new QFrame(this);
    addTab(mPlaceholder, tr("Placeholder")); //TODO: Proper title
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
    //load the new file
    TraceWidget* newView = new TraceWidget(this);
    addTab(newView, tr("Trace")); //TODO: Proper title
    setCurrentIndex(count() - 1);
    emit newView->openSlot(browse.path);
}

void TraceManager::closeTab(int index)
{
    auto view = qobject_cast<TraceWidget*>(widget(index));
    if(view)
    {
        removeTab(index);
        delete view;
    }
    else
    {
        // Placeholder tab
        return;
    }
}

void TraceManager::closeAllTabs()
{
    while(count() > 1)
    {
        closeTab(count() - 1);
    }
}
