# Modification: 2016.01; USC
# Updated included modules to support Qt 5.7 
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4) {
   QT += widgets
   qtHaveModule(webengine) {
      QT += webenginewidgets
   } else {
      qtHaveModule(webkit) {
         DEFINES += NEED_WEBKIT_LEGACY
         QT += webkitwidgets
      } else {
         DEFINES += NO_WEB_SUPPORT
         warning(Compiling with no web support. Visual differencing integration will be disabled!)
      }
   }
} else {
   DEFINES += NEED_WEBKIT_LEGACY
   QT += webkit
}

CONFIG	+= qt warn_on debug_and_release no_batch
# Modification: 2016.01; USC
# Fixed mingw 4.9.1 compilation errors
CONFIG  += c++11

TEMPLATE	= app
LANGUAGE	= C++

DEFINES	+= QTGUI

HEADERS += \
	GAsciiDialog.h \
	GExtensionDialog.h \
	GMainWindow.h \
	GSideBySideDialog.h \
	GTableDialog.h \
	GUtil.h\
	../src/CAdaCounter.h \
	../src/CBashCounter.h \
	../src/CBatchCounter.h \
	../src/cc_main.h \
	../src/CCCounter.h \
	../src/CCFScriptCounter.h \
	../src/CCJavaCsScalaCounter.h \
	../src/CCodeCounter.h \
	../src/CColdFusionCounter.h \
	../src/CCsharpCounter.h \
	../src/CCshCounter.h \
	../src/CCssCounter.h \
	../src/CDataCounter.h \
	../src/CFortranCounter.h \
	../src/CHtmlCounter.h \
	../src/CJavaCounter.h \
	../src/CJavascriptCounter.h \
	../src/CMakefileCounter.h \
	../src/CMatlabCounter.h \
	../src/CMidasCounter.h \
	../src/CmpMngr.h \
	../src/CNeXtMidasCounter.h \
        ../src/CObjCCounter.h \
	../src/CPascalCounter.h \
	../src/CPerlCounter.h \
	../src/CPhpCounter.h \
	../src/CPythonCounter.h \
	../src/CRubyCounter.h \
	../src/CSqlCounter.h \
	../src/CTagCounter.h \
	../src/CUtil.h \
	../src/CVbCounter.h \
	../src/CVbscriptCounter.h \
	../src/CVerilogCounter.h \
	../src/CVHDLCounter.h \
	../src/CWebCounter.h \
	../src/CXMidasCounter.h \
	../src/CXmlCounter.h \
	../src/DiffTool.h \
	../src/MainObject.h \
	../src/UserIF.h \
        ../src/CmpMngrHtml.h \
        ../src/CAssemblyCounter.h \
        ../src/CIdlCounter.h \
	../src/UCCThread.h \
	../src/UCCGlobals.h \
	../src/UCCFilesOut.h \
	../src/UCCExceptDump.h \
	../src/sema.h \
	../src/LangUtils.h \
	../src/CScalaCounter.h \
	../src/CCobolCounter.h \
        ../src/UCCAfterLibraryIncludes.h \
        ../src/UCCBeforeLibraryIncludes.h \
	# Modification: 2017.02
	../src/FunctionParser.h \
        UCCWorker.h

SOURCES += \
	GAsciiDialog.cpp \
	GExtensionDialog.cpp \
	GMainWindow.cpp \
	GSideBySideDialog.cpp \
	GTableDialog.cpp \
	GUtil.cpp\
	../src/CAdaCounter.cpp \
	../src/CBashCounter.cpp \
	../src/CBatchCounter.cpp \
	../src/cc_main.cpp \
	../src/CCCounter.cpp \
	../src/CCFScriptCounter.cpp \
	../src/CCJavaCsScalaCounter.cpp \
	../src/CCodeCounter.cpp \
	../src/CColdFusionCounter.cpp \
	../src/CCsharpCounter.cpp \
	../src/CCshCounter.cpp \
	../src/CCssCounter.cpp \
	../src/CDataCounter.cpp \
	../src/CFortranCounter.cpp \
	../src/CHtmlCounter.cpp \
	../src/CJavaCounter.cpp \
	../src/CJavascriptCounter.cpp \
	../src/CMakefileCounter.cpp \
	../src/CMatlabCounter.cpp \
	../src/CMidasCounter.cpp \
	../src/CmpMngr.cpp \
	../src/CNeXtMidasCounter.cpp \
        ../src/CObjCCounter.cpp \
	../src/CPascalCounter.cpp \
	../src/CPerlCounter.cpp \
	../src/CPhpCounter.cpp \
	../src/CPythonCounter.cpp \
	../src/CRubyCounter.cpp \
	../src/CSqlCounter.cpp \
	../src/CTagCounter.cpp \
	../src/CUtil.cpp \
	../src/CVbCounter.cpp \
	../src/CVbscriptCounter.cpp \
	../src/CVerilogCounter.cpp \
	../src/CVHDLCounter.cpp \
	../src/CWebCounter.cpp \
	../src/CXMidasCounter.cpp \
	../src/CXmlCounter.cpp \
	../src/DiffTool.cpp \
	../src/MainObject.cpp \
	../src/UserIF.cpp \
        ../src/CmpMngrHtml.cpp \
        ../src/CAssemblyCounter.cpp \
        ../src/CIdlCounter.cpp \
        ../src/UCCThread.cpp \
	../src/UCCGlobals.cpp \
	../src/UCCFilesOut.cpp \
	../src/UCCExceptDump.cpp \
	../src/LangUtils.cpp \
	../src/CScalaCounter.cpp \
	../src/CCobolCounter.cpp \
	../src/main.cpp \
	# Modification: 2017.02
	../src/FunctionParser.cpp \
        Qt_main.cpp \
        UCCWorker.cpp

FORMS += \
	GAsciiDialog.ui \
	GExtensionDialog.ui \
	GMainWindow.ui \
	GTableDialog.ui 

RESOURCES += \
	gucc.qrc

win32 {
	TARGET = GUCC
	UI_DIR = GeneratedFiles
	RCC_DIR = GeneratedFiles
	RC_FILE = GUCC.rc
	#Modification: 2016.01; USC
        #Fixed mingw 4.9.1 compilation errors
        DEFINES += WIN32
}
unix {
	UI_DIR = .ui
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
	DEFINES += UNIX
}
macx {
	TARGET = GUCC
	RC_FILE = images/gucc.icns
	DEFINES += UNIX
}
