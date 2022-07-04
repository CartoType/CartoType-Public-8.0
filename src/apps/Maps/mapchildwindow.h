#pragma once

#include <QtGui>
#include <QWidget>
#include <QMdiSubWindow>

#include "mapform.h"

class MapChildWindow : public QMdiSubWindow
    {
    public:
    explicit MapChildWindow(QWidget* aParent,MainWindow& aMainWindow,const std::vector<QString>& aMapFileNameArray);
    ~MapChildWindow();
    void SetView(const CartoType::ViewState& aViewState);

    MapForm* m_map_form;
    };
