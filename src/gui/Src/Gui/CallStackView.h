#pragma once

#include "StdIconTable.h"
class CommonActions;

class CallStackView : public StdIconTable
{
    Q_OBJECT
public:
    explicit CallStackView(StdTable* parent = nullptr);
    void setupContextMenu();
    duint getSelectionVa();

protected:
    QString paintContent(QPainter* painter, duint row, duint col, int x, int y, int w, int h) override;

protected slots:
    void updateCallStack();
    void contextMenuSlot(const QPoint pos);
    void followAddress();
    void followTo();
    void followFrom();
    void showSuspectedCallStack();
    void followInThreads();
    void renameThread();

private:
    enum
    {
        ColThread = 0,
        ColAddress,
        ColTo,
        ColFrom,
        ColSize,
        ColParty,
        ColComment
    };

    MenuBuilder* mMenuBuilder;
    CommonActions* mCommonActions;
    bool isSelectionValid();
    bool CallStackView::isThreadHeaderSelected();
};
