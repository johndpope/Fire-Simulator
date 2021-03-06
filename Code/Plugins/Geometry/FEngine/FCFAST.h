#pragma once

#include "Core/Spatial/NESpatialGroup.h"
#include "Core/Utility/FilePath.h"
#include "Base/NENodeFactory.h"
#include "Utility/NEColorRamp.h"

class FCFASTPrivate;

class FDYNCEXP_PLUGINS FCFAST: public NESpatialGroup
{
    Q_OBJECT


    NE_PROP_GROUP_START(Simulation, "Simulation", "Simulations", true)
    NE_PROP_RW(double, SimTime, "Total Sim Time", "Simulation Time", true, )
    NE_PROP_RW(double, TextOutputInt, "Text Output Interval", "Text output interval", true, )
    NE_PROP_RW(double, SpreadSheetOutputInt, "Spreadsheet Output Interval", "Spreadsheet output interval", true, )
    NE_PROP_RW(double, SmokeViewOutputInt, "SmokeView Output Interval", "Smokeview output interval", true, )
    NE_PROP_RW(double, IntTemp, "Int. Temperature","Interior temperature", true, )
    NE_PROP_RW(double, ExtTemp, "Ext. Temperature","Exterior temperature", true, )
    NE_PROP_RW(double, Humidity, "Humidity", "Humidity", true, )
    NE_PROP_RW(double, Pressure, "Pressure", "Pressure", true, )
    NE_PROP_RW(double, MaxTimeStep, "Max. Time Step", "Maximum time step", true, )
    NE_PROP_RW(double, LowerO2Limit, "Lower Oxygen Limit", "Lower oxygen limit", true, )
    NE_PROP_RW(bool, Adiabatic, "Adiabatic Surfaces", "Adiabatic surfaces", true, )
    NE_PROP_GROUP_END(Simulation)

    NE_PROP_GROUP_START(Visualization, "Visualization", "Visualization", true)
    NE_PROP_RW(NEColorRamp, ColorTemp, "Color Temp", "Color Ramp Describing the Min/Max emp", true,)
    NE_PROP_RW(bool, Visualize, "Visualize", "Visualize", true,)
    NE_PROP_RW(bool, XVisPlane, "X Plane", "X Plane Visualize", true,)
    NE_PROP_RW(double, XVisPlanePos, "X Plane Pos", "X Plane Visualize Slice Position (0-1)", true,)
    NE_PROP_RW(bool, YVisPlane, "Y Plane", "Y Plane Visualize", true,)
    NE_PROP_RW(double, YVisPlanePos, "X Plane Pos", "X Plane Visualize Slice Position (0-1)", true,)
    NE_PROP_RW(bool, ZVisPlane, "Z Plane", "Z Plane Visualize", true,)
    NE_PROP_RW(double, ZVisPlanePos, "Z Plane Pos", "Z Plane Visualize Slice Position (0-1)", true,)


    NE_PROP_RW(FilePath, CompartmentData, "Compartment Results", "CFAST Compartment Data Output", true,)
    NE_PROP_RW(FilePath, WallData, "Wall Results", "CFAST Compartment Wall Output", true,)
    NE_PROP_RW(FilePath, SurfaceData, "Surface Results", "CFAST Object Surfaces Data Output", true,)
    NE_PROP_GROUP_END(Visualization)

public:

    FCFAST( NENodeFactory* factory  );
    virtual ~FCFAST();

    uint init(uint initMode = NE::kInitOnCreate) override;
    void resetProperties() override;

    QVector<QPair<qreal, QColor> > getColorList() const;

public slots:
    QString toJson();
    void fromJson( QString jsonStr );

private:
    NE_DECL_PRIVATE(FCFAST)
};

class FCFASTFactory : public NENodeFactory
{
    Q_OBJECT
    Q_INTERFACES(NENodeFactory)

public:
    FCFASTFactory(NEManager* m = 0);
    virtual ~FCFASTFactory();

    NE_DERIVED(NENodeFactory, Public)
};

