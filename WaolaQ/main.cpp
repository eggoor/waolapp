#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	//Q_IMPORT_QML_PLUGIN(WaolaQtPlugin)

	QQmlApplicationEngine engine;
	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
		&app,
		[]() { QCoreApplication::exit(-1); },
		Qt::QueuedConnection);

#if defined(__linux__)
	constexpr auto uri = "waolaq";
#else
	constexpr auto uri = "WaolaQ";
#endif

	engine.loadFromModule(uri, "Main");

	return app.exec();
}
