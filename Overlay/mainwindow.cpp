#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , udpSocket(new QUdpSocket(this))
    , udpServerIP("192.168.1.9") // DE10’s IP address
    , udpServerPort(5005)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_base_clicked()
{
    // https://doc.qt.io/qt-5/qfiledialog.html
    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Base Image"),          // window text
                                            "",                             // starting directory
                                            tr("Bitmap (*.bmp)"));          // file type
    if (!fileName.isEmpty())    // if file is not empty
    {
        // load file
        base_image.load(fileName);
        // sanitize file
        base_image = base_image.convertToFormat(QImage::Format_Grayscale8);
        base_image = base_image.convertToFormat(QImage::Format_Indexed8);
        // display file
        int h, w;
        h = ui -> label_base -> height();
        w = ui -> label_base -> width();
        ui -> label_base -> setPixmap(QPixmap::fromImage(base_image).scaled(w,h,Qt::KeepAspectRatio));
        // reset active-high boolean that disables update_output
        base_not_set=0;
    }
    update_output();
}

void MainWindow::on_pushButton_overlay_clicked()
{
    // https://doc.qt.io/qt-5/qfiledialog.html
    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Overlay Image"),       // window text
                                            "",                             // starting directory
                                            tr("Bitmap (*.bmp)"));          // file type
    if (!fileName.isEmpty())    // if file is not empty
    {
        // load file
        overlay_image.load(fileName);
        // sanitize file
        overlay_image = overlay_image.convertToFormat(QImage::Format_Grayscale8);
        overlay_image = overlay_image.convertToFormat(QImage::Format_Indexed8);
        // display file
        int h, w;
        h = ui -> label_overlay -> height();
        w = ui -> label_overlay -> width();
        ui -> label_overlay -> setPixmap(QPixmap::fromImage(overlay_image).scaled(w,h,Qt::KeepAspectRatio));
        // reset active-high boolean that disables update_output
        overlay_not_set=0;
    }
    update_output();
}

void MainWindow::on_pushButton_composite_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save COmposite Image"), // window text
                                                    "",                         // starting directory
                                                    tr("Bitmap (*.bmp)"));      // file type
    composite_image.save(fileName);
}

void MainWindow::update_output()
{
    sendUdpInteger(0x20,status);
    if(base_not_set || overlay_not_set)
    {return;}   // do not run unless both base and overlay are set

    // get overlay dimensions
    int width = overlay_image.width();
    int height = overlay_image.height();
    // find maximum of each dimension between base and overlay
    int w = qMax(base_image.width(), width);
    int h = qMax(base_image.height(), height);
    // copy base image,expanded to maximized dimensions using '0' to fill
    composite_image = base_image.copy(0,0,w,h);

    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            composite_image.setPixel(x,y,overlay_image.pixelIndex(x,y));    // copy overlay pixels over base image
        }
    }
    // display file
    h = ui -> label_overlay -> height();
    w = ui -> label_overlay -> width();
    ui -> label_composite -> setPixmap(QPixmap::fromImage(composite_image).scaled(w,h,Qt::KeepAspectRatio));
}

void MainWindow::on_checkBox_overlay_toggle_stateChanged(int arg1)
{
    if (arg1){status = 0x1002;}
    else {status = 0x1001;}
    sendUdpInteger(0x20,status);
}

void MainWindow::on_pushButton_overlay_send_clicked()
{
    if(overlay_image.isNull())
    {return;}   // do not run unless image has been set
    sendUdpImage(0x12,overlay_image);
    sendUdpInteger(0x20,status);
}

void MainWindow::on_pushButton_base_send_clicked()
{
    if(base_image.isNull())
    {return;}   // do not run unless image has been set
    sendUdpImage(0x11,base_image);
    sendUdpInteger(0x20,status);
}


void MainWindow::sendUdpImage(quint32 messageId, const QImage &image) {
    // Convert the image to a QByteArray (e.g., PNG format)
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "BMP"); // Save the image in BMP format

    // Use sendUdpData to send the serialized image
    sendUdpData(messageId, data);

    qDebug() << "Image sent as UDP data with Message ID:" << messageId;
}


void MainWindow::sendUdpInteger(quint32 messageId, quint32 integerValue) {
    // Serialize the integer into a QByteArray
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream.setByteOrder(QDataStream::LittleEndian); // Set to little-endian format

    // Write the integer value into the stream
    stream << integerValue;

    // Use sendUdpData to send the serialized integer
    sendUdpData(messageId, data);

    qDebug() << "Integer value" << integerValue << "sent as UDP data with Message ID:" << messageId;
}

void MainWindow::sendUdpData(quint32 messageId, const QByteArray &data) {
    if (data.isEmpty()) {
        qDebug() << "No data to send for Message ID:" << messageId;
        return;
    }

    const int chunkSize = 1012; // 12 bytes for headers, remaining for data
    int totalChunks = (data.size() + chunkSize - 1) / chunkSize; // Calculate total chunks

    for (int i = 0; i < totalChunks; ++i) {
        QByteArray chunkPacket;
        QDataStream stream(&chunkPacket, QIODevice::WriteOnly);

        stream.setByteOrder(QDataStream::LittleEndian); // Set to little-endian format

        stream << messageId;          // Message ID -- 4 bytes
        stream << quint32(i);         // Sequence number -- 4 bytes
        stream << quint32(totalChunks); // Total chunks -- 4 bytes
        stream << data.mid(i * chunkSize, chunkSize); // Extract chunk data (up to `chunkSize`)

        udpSocket->writeDatagram(chunkPacket, QHostAddress(udpServerIP), udpServerPort);

        qDebug() << "Chunk" << i + 1 << "of" << totalChunks << "sent for Message ID:" << messageId;
        qDebug() << "First byte of data in chunk: " << data.mid(i * chunkSize, 1).toHex();
    }
}
