#pragma once

#include <QDialog>

namespace Ui
    {
    class RouteInstructionsDialog;
    }

class RouteInstructionsDialog: public QDialog
    {
    Q_OBJECT

    public:
    explicit RouteInstructionsDialog(QWidget* aParent = 0);
    ~RouteInstructionsDialog();

    Ui::RouteInstructionsDialog* m_ui;
    };
