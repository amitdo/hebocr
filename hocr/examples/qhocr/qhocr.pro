TMP_DIR      = .tmp
UI_DIR       = .tmp
MOC_DIR      = .tmp
OBJECTS_DIR  = .tmp

LIBS += ../../src/.libs/libhocr.so

TEMPLATE	 = app

FORMS		+= \
	ui/mainwindow.ui

INCLUDEPATH	+= \
	src	

HEADERS		+= \
	src/mainwindow.h 
	
SOURCES		+= \
	src/main.cpp \
	src/mainwindow.cpp