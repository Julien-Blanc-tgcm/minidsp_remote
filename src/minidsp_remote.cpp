#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QtQml>

#include <sailfishapp.h>

#include "nanoavrbasicsettings.h"
#include "nanoavrclient.h"

int main(int argc, char *argv[])
{
	// SailfishApp::main() will display "qml/minidsp_remote.qml", if you need more
	// control over initialization, you can use:
	//
	//   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
	//   - SailfishApp::createView() to get a new QQuickView * instance
	//   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
	//   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
	//
	// To display the view, call "show()" (will show fullscreen on device).

	auto qmlprefix = "harbour.eu.tgcm.minidsp_remote";
	qmlRegisterType<eu::tgcm::minidsp_remote::NanoAVRBasicSettings>(qmlprefix, 1, 0, "NanoAVRBasicSettings");
	qmlRegisterType<eu::tgcm::minidsp_remote::NanoAVRClient>(qmlprefix, 1, 0, "NanoAVRClient");
	return SailfishApp::main(argc, argv);
}
