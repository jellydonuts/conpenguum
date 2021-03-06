#include "ServerEngine.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv, false);
	ServerEngine *se = ServerEngine::GetInstance();

	if (!se->Start())
		return 0;

	return app.exec();
}
