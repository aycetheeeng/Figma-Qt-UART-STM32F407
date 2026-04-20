#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class SerialHandler : public QObject
{
    Q_OBJECT
    // QML'deki "visible" özelliğine bağlayacağımız özellikler
    Q_PROPERTY(bool leftSignal READ leftSignal NOTIFY leftSignalChanged)
    Q_PROPERTY(bool rightSignal READ rightSignal NOTIFY rightSignalChanged)

public:
    explicit SerialHandler(QObject *parent = nullptr);

    // Değişkenleri QML'e okutan fonksiyonlar (Getter)
    bool leftSignal() const { return m_leftSignal; }
    bool rightSignal() const { return m_rightSignal; }

signals:
    void leftSignalChanged();
    void rightSignalChanged();

private slots:
    void readData();

private:
    QSerialPort *serial;
    // HATA BURADAN KAYNAKLANIYOR: Bu değişkenleri buraya eklemelisin
    bool m_leftSignal = false;
    bool m_rightSignal = false;
};

#endif
