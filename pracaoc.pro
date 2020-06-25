# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src
# El destino es una aplicaci??n: ../bin/pracoc

FORMS += mainForm.ui \
           escalaForm.ui \
           filtroForm.ui \
           umbralForm.ui
HEADERS += pracaoc.h \
           imageprocess.h 
SOURCES += pracaoc.cpp \
           main.cpp \
           imageprocess.cpp 
TEMPLATE = app
CONFIG += release \
warn_on \
thread \
qt

QMAKE_CXXFLAGS_DEBUG -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS  += -m32
QMAKE_CFLAGS_DEBUG -= -O2
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS  += -m32
QMAKE_LFLAGS  += -m32
QMAKE_LIBDIR  += /usr/lib/i386-linux-gnu /usr/lib32


TARGET = pracAOC
