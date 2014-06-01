QT += core quick
# To make threaded gl check...
QT += core-private gui-private

HEADERS += \
    threadrenderer.h \
    mainwindow.h \
    renderthread.h \
    texturenode.h \
    datamanager.h \
    renderer.h \
    rendererdata.h \
    mesh.h \
    quadmesh.h

SOURCES += main.cpp \
    mainwindow.cpp \
    renderthread.cpp \
    texturenode.cpp \
    threadrenderer.cpp \
    datamanager.cpp \
    renderer.cpp \
    mesh.cpp \
    quadmesh.cpp

RESOURCES += \
    content/windows/windows.qrc \
    content/shaders/shaders.qrc

OTHER_FILES += \
    content/windows/main.qml \
    content/windows/error.qml \
    content/shaders/collect_data_pass.fs \
    content/shaders/collect_data_pass.vs \
    content/shaders/calculate_ssao_pass.fs \
    content/shaders/calculate_ssao_pass.vs \
    content/shaders/filter_ssao_pass.fs \
    content/shaders/filter_ssao_pass.vs \
    content/shaders/render_pass.fs \
    content/shaders/render_pass.vs \
    content/shaders/compose_pass.fs \
    content/shaders/compose_pass.vs

INCLUDEPATH += \
    $$PWD/include/tools \
    $$PWD/include/glew/include \
    $$PWD/include/assimp/include \
    $$PWD/include/freeimage/include

DEPENDPATH += \
    $$PWD/include/tools \
    $$PWD/include/glew/include \
    $$PWD/include/assimp/include \
    $$PWD/include/freeimage/include

win32: LIBS += -L$$PWD/include/assimp/lib/assimp_release-dll_win32/ -lassimp
win32: LIBS += -L$$PWD/include/glew/lib/Release/Win32/ -lglew32
win32: LIBS += -L$$PWD/include/freeimage/lib/win32/ -lFreeImage
win32: LIBS += -lglu32 -lopengl32
#win32: LIBS += -lglew32s -lglu32 -lopengl32




