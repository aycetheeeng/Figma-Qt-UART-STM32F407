#include "serialhandler.h"
#include <QDebug>

SerialHandler::SerialHandler(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    serial->setPortName("COM7"); // AYGIT YÖNETİCİSİNDEN BAKTIĞIN PORTU BURAYA YAZ (Örn: COM4)
    serial->setBaudRate(9600);

    if (serial->open(QIODevice::ReadOnly)) {
        qDebug() << "STM32 Baglantisi Basarili!";
        connect(serial, &QSerialPort::readyRead, this, &SerialHandler::readData);
    } else {
        qDebug() << "HATA: Port acilamadi! Kablo takili mi?";
    }
}

void SerialHandler::readData() {
    QByteArray rawData = serial->readAll();
    QString hexData = rawData.toHex();

    qDebug() << "Gelen Hex Veri:" << hexData;

    // ÖNCE SİNYALLERİ KONTROL ET (Çünkü paketlerin içinde e0 da var)

    // 1. DURUM: Eğer içinde 1c1c varsa SOL YAK
    if (hexData.contains("1c1c")) {
        m_leftSignal = true;
        m_rightSignal = false;
        emit leftSignalChanged();
        emit rightSignalChanged();
        qDebug() << ">>> SOL SINYAL YANDI (1c1c yakalandı)";
    }
    // 2. DURUM: Eğer içinde 001c varsa SAĞ YAK
    else if (hexData.contains("001c")) {
        m_rightSignal = true;
        m_leftSignal = false;
        emit rightSignalChanged();
        emit leftSignalChanged();
        qDebug() << ">>> SAG SINYAL YANDI (001c yakalandı)";
    }
    // 3. DURUM: Eğer veri sadece "e0" ise veya yukarıdakiler YOKSA kapat
    else if (hexData == "e0" || (!hexData.contains("1c1c") && !hexData.contains("001c"))) {
        m_leftSignal = false;
        m_rightSignal = false;
        emit leftSignalChanged();
        emit rightSignalChanged();
        qDebug() << ">>> SINYALLER KAPANDI (Sadece e0 veya veri yok)";
    }
}
