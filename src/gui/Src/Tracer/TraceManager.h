#pragma once

#include <QTabWidget>
#include <QPushButton>
#include <QMap>
#include "TraceWidget.h"

class TraceManager : public QTabWidget
{
    Q_OBJECT
public:
    explicit TraceManager(QWidget* parent = 0);
    ~TraceManager();

public slots:
    void open();
    void openSlot(const QString &);
    void closeTab(int index);
    void closeAllTabs();

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    QPushButton* mOpen;
    QPushButton* mCloseAllTabs;
    QList<QObject*> mViewsToDelete;
};
