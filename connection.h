#ifndef CONNECTION_H
#define CONNECTION_H

#include <QDialog>

class connection
{
public:
    connection();
    QString hostname() const;
    quint16 port() const;

private:
    QString uHostname;
    quint16 uPort;
};

inline QString connection::hostname() const{
    return uHostname;
}

inline quint16 connection::port() const{
    return uPort;
}

#endif // CONNECTION_H
