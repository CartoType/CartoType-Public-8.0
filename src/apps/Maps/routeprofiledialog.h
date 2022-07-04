#pragma once

#include <QDialog>
#include <cartotype.h>

namespace Ui {
class RouteProfileDialog;
}

class RouteProfileDialog: public QDialog
    {
    Q_OBJECT

    public:
    explicit RouteProfileDialog(QWidget *aParent,const CartoType::RouteProfile& aRouteProfile,bool aMetricUnits);
    ~RouteProfileDialog();
    const CartoType::RouteProfile& RouteProfile();

    Ui::RouteProfileDialog* m_ui;

    private slots:
    void on_roadTypeList_currentTextChanged(const QString &aText);
    void on_speedSlider_valueChanged(int aValue);
    void on_bonusSlider_valueChanged(int aValue);
    void on_tollSlider_valueChanged(int aValue);
    void on_copyDrive_clicked();
    void on_copyCycle_clicked();
    void on_copyWalk_clicked();
    void on_copyHike_clicked();
    void on_restrictionCheckBox_toggled(bool aChecked);
    void on_onewayCheckBox_toggled(bool aChecked);
    void on_vehicleTypeList_currentIndexChanged(int aIndex);
    void on_vehicleWeightSlider_valueChanged(int aValue);
    void on_vehicleHeightSlider_valueChanged(int aValue);

    private:
    void Set(const CartoType::RouteProfile& aRouteProfile);
    void SetRoadType();
    void SetRoadTypeLabel();
    void SetSpeedLabel();
    void SetBonusLabel();
    void SetTollLabel();
    void SetWeightLabel();
    void SetHeightLabel();
    void SetRestrictions();
    void Copy(CartoType::RouteProfileType aType);

    CartoType::RouteProfile m_route_profile;
    bool m_metric_units;
    };
