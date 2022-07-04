#pragma once

#include <QDialog>

namespace Ui
    {
    class ScaleDialog;
    }

class ScaleDialog: public QDialog
    {
    Q_OBJECT

    public:
    explicit ScaleDialog(QWidget* aParent = nullptr);
    ~ScaleDialog();

    Ui::ScaleDialog* m_ui;
    };
