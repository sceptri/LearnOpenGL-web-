TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/sources/main.cpp\
    src/sources/shader_s.cpp \
    src/sources/stb_image.cpp

unix|win32: LIBS += -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

INCLUDEPATH +=  include \

HEADERS += \
    src/headers/shader_s.h \

include(res/shaders/shaders.pri)
include(include/include.pri)
