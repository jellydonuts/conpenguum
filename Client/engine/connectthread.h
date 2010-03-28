#ifndef CONNECTTHREAD_H_
#define CONNECTTHREAD_H_

#include <QThread>

class ConnectThread : public QThread
{
	Q_OBJECT

public:
	ConnectThread(std::string name, std::string ip, std::string port);
	void run();

signals:
	void serverConnect(int);
	void errorConnect();

private:
	std::string name_, ip_, port_;
};

#endif /* CONNECTTHREAD_H_ */