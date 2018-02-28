TEMPLATE = subdirs

QMAKE_CXXFLAGS += -std=c++17
CONFIG += c++17


SUBDIRS += \
    Gui \
    ImageS

Gui.depends = ImageS

