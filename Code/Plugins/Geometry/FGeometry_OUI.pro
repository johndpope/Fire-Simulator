#include(../../QMake/common.pri)

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS                   += FEngine
#SUBDIRS                   += FGeo

FEngine.file   = FEngine/FEngine_OUI.pro
#FGeo.file      = FGeo/FGeo.pro
