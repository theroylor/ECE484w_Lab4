#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtGlobal>
#include <QTextStream>

#include <QVector>
#include <QtMath>
#include <QElapsedTimer>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDebug>
#include <QBuffer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void asd();

    void image_load();

private slots:
    void on_pushButton_base_clicked(); // load base image

    void on_pushButton_overlay_clicked(); // load overlay image

    void on_pushButton_composite_clicked(); // save resulting image

    void update_output(); // update composite image

    void on_checkBox_overlay_toggle_stateChanged(int arg1);

    void on_pushButton_overlay_send_clicked();

    void on_pushButton_base_send_clicked();

private:
    QUdpSocket *udpSocket;
    QString udpServerIP;
    quint16 udpServerPort;
    Ui::MainWindow *ui;
    bool base_not_set=1;
    bool overlay_not_set=1;
    QImage base_image, overlay_image, composite_image;
    void sendUdpImage(quint32 messageId, const QImage &image);
    void sendUdpInteger(quint32 messageId, quint32 integerValue);
    void sendUdpData(quint32 messageId, const QByteArray &data);
    quint32 status=0x1002;
};
#endif // MAINWINDOW_H
