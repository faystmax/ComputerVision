TEMPLATE = subdirs

CONFIG += c++17

SUBDIRS += \
    Gui \
    ImageS

Gui.depends = ImageS

