TEMPLATE = subdirs

SUBDIRS += \
    Gui \
    ImageS \
    linalg

Gui.depends = ImageS
ImageS.depends = linalg
