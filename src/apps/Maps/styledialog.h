#pragma once

#include <QDialog>
#include <QTreeWidgetItem>
#include <QAbstractButton>
#include <rapidxml.hpp>
#include <cartotype.h>
#include "stylemodel.h"
#include "styleitem.h"

namespace Ui
    {
    class StyleDialog;
    }

class StyleDialog: public QDialog
    {
    Q_OBJECT

    public:
    StyleDialog(QWidget* aParent,CartoType::Framework& aFramework,const std::string& aStyleSheet);
    ~StyleDialog();
    bool HaveError() const { return m_style_model.HaveError(); }
    const std::string& ErrorMessage() const { return m_style_model.ErrorMessage(); }
    const std::string& ErrorLocation() const { return m_style_model.ErrorLocation(); }
    std::string EditedStyleSheetText() const;
    std::string OriginalStyleSheetText() const;

    private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_styleTree_doubleClicked(const QModelIndex &index);
    void on_buttonBox_clicked(QAbstractButton* aButton);

    signals:
    void ApplyStyleSheet(std::string aStyleSheet);

    private:
    void EditLine(CStyleItem& aStyleItem);
    void EditPolygon(CStyleItem& aStyleItem);
    void EditSvg(CStyleItem& aStyleItem);
    void EditAttributes(CStyleItem& aStyleItem);

    Ui::StyleDialog *m_ui;
    CartoType::Framework& m_framework;
    CartoType::Legend m_legend;
    CStyleModel m_style_model;
    CStyleItemDelegate m_style_item_delegate;
    };
