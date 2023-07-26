#include "widget.h"

#include <QGridLayout>
#include <QFileDialog>
#include <qDebug>
#include <QGraphicsPixmapItem>

void Widget::CheckMem(){

    if (access == true){

        setScene->removeItem(item);

        delete item;
        item = nullptr;
        setScene->clear();

        ReturnImage(number);
        item = new QGraphicsPixmapItem(pixmap);
        qDebug() << "accessView :" << accessView;


        float scaleFactor = qMin(viewImage->width() / item->boundingRect().width(), viewImage->height() / item->boundingRect().height());

        setScene->addItem(item);

        item->setTransformOriginPoint(item->boundingRect().center());

        item->setScale(scaleFactor);

        QPointF center = viewImage->viewport()->rect().center();
        QPointF itemCenter = item->mapToScene(item->boundingRect().center());
        QPointF offset = center - itemCenter;
        item->setPos(item->pos() + offset);

        viewImage->setScene(setScene);
    }
}
    QImage Widget::ConvertToBlackAndWhite(const QImage& colorImage) {

        QImage blackAndWhiteImage(colorImage.size(), QImage::Format_RGB32);

            for (int y = 0; y < colorImage.height(); ++y) {
                for (int x = 0; x < colorImage.width(); ++x) {
                    // Получаем цвет пикселя в RGB формате
                    QColor color = QColor::fromRgb(colorImage.pixel(x, y));

                    // Вычисляем яркость пикселя по формуле Y = 0.299R + 0.587G + 0.114B
                    int brightness = 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue();

                    // Создаем новый цвет пикселя, используя яркость для всех компонентов RGB
                    QColor newColor(brightness, brightness, brightness);

                    // Устанавливаем новый цвет пикселя в черно-белое изображение
                    blackAndWhiteImage.setPixelColor(x, y, newColor);
                }
            }
            return blackAndWhiteImage;
    }

    QImage Widget::FilterImage3x3(QImage inputImage)
    {
        int width = inputImage.width();
        int height = inputImage.height();

        QImage outputImage(width, height, QImage::Format_RGB32);

        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                QRgb pixel00 = inputImage.pixel(x - 1, y - 1);
                QRgb pixel01 = inputImage.pixel(x, y - 1);
                QRgb pixel02 = inputImage.pixel(x + 1, y - 1);
                QRgb pixel10 = inputImage.pixel(x - 1, y);
                QRgb pixel11 = inputImage.pixel(x, y);
                QRgb pixel12 = inputImage.pixel(x + 1, y);
                QRgb pixel20 = inputImage.pixel(x - 1, y + 1);
                QRgb pixel21 = inputImage.pixel(x, y + 1);
                QRgb pixel22 = inputImage.pixel(x + 1, y + 1);

                int red = (qRed(pixel00) + qRed(pixel01) + qRed(pixel02) +
                           qRed(pixel10) + qRed(pixel11) + qRed(pixel12) +
                           qRed(pixel20) + qRed(pixel21) + qRed(pixel22)) / 9;
                int green = (qGreen(pixel00) + qGreen(pixel01) + qGreen(pixel02) +
                             qGreen(pixel10) + qGreen(pixel11) + qGreen(pixel12) +
                             qGreen(pixel20) + qGreen(pixel21) + qGreen(pixel22)) / 9;
                int blue = (qBlue(pixel00) + qBlue(pixel01) + qBlue(pixel02) +
                            qBlue(pixel10) + qBlue(pixel11) + qBlue(pixel12) +
                            qBlue(pixel20) + qBlue(pixel21) + qBlue(pixel22)) / 9;

                outputImage.setPixel(x, y, qRgb(red, green, blue));
            }
        }
        return outputImage;
    }

    void Widget::CreateButton(QPushButton *button, int width, int height){
        button->setFixedSize(width, height);
        button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

    QPixmap Widget::ReturnImage(int unit)
    {
        pixmap.load(file_name);

        switch(unit){
        case 0:
            return pixmap;
        case 2:
            return pixmap = QPixmap::fromImage(ConvertToBlackAndWhite(image));
        case 1:
            return pixmap = QPixmap::fromImage(FilterImage3x3(image));

        default:
            return pixmap;
        }
    }

    void Widget::BlackInWhite(){

        number =1;
        accessView = true;
        CheckMem();
    }

    void Widget::LoadButtonImage(){

        QString Path = QFileDialog::getOpenFileName(this); // далогов

        if (Path != file_name){

            file_name.clear();
            file_name = Path;

            pixmap = QPixmap();
            image = QImage();

            pixmap.load(file_name);
            image.load(file_name);

            access = true;
        }
    }
    void Widget::LinearImageFiltering(){
        number = 2;
        accessView = false;
        CheckMem();
    }

    void Widget::ShowImageButton(){
        number = 0;
        accessView = false;
        CheckMem();
    }

    void Widget::SaveImageButton(){
        if(!pixmapFilt.isNull()){
        QString fileNameSave = QFileDialog::getSaveFileName(nullptr, "Сохранить изображение", "", "JPEG (*.jpg);;PNG (*.png)");
        pixmapFilt.save(fileNameSave);}
    }
    /////////////////////////////////////////////////////////////////////////////////////////////

    Widget::Widget(QWidget *parent)
        : QWidget(parent){
        QGridLayout* body_grid = new QGridLayout(this);

        QPushButton* open = new QPushButton("Open" );
        QPushButton* save = new QPushButton("Save");
        QPushButton* show = new QPushButton("Show");
        QPushButton* linearImageFiltering = new QPushButton("Linear filtering");
        QPushButton* toBlackAndWhite = new QPushButton("To black and white");

        CreateButton(open,100, 30);
        body_grid->addWidget(open, 1, 3, 1, 1);

        CreateButton(save,100, 30);
        body_grid->addWidget(save, 2, 3, 1, 1);

        CreateButton(show,100, 30);
        body_grid->addWidget(show, 3, 3, 1, 1);

        CreateButton(toBlackAndWhite,100, 30);
        body_grid->addWidget(toBlackAndWhite, 4, 3, 1, 1);

        CreateButton(linearImageFiltering,100, 30);
        body_grid->addWidget(linearImageFiltering, 4, 2, 1, 1);

        connect(open, SIGNAL(clicked()), this, SLOT(LoadButtonImage()));
        connect(show, SIGNAL(clicked()), this, SLOT(ShowImageButton()));
        connect(save, SIGNAL(clicked()), this, SLOT(SaveImageButton()));
        connect(toBlackAndWhite, SIGNAL(clicked()), this, SLOT(BlackInWhite()));
        connect(toBlackAndWhite, SIGNAL(clicked()), this, SLOT(BlackInWhite()));

        setScene = new QGraphicsScene(viewImage);
        viewImage = new ImageView;
        viewImage->setScene(setScene);
        while (viewImage == nullptr) {viewImage = new ImageView;}

        body_grid->addWidget(viewImage, 1, 1, 5, 1);
        setLayout(body_grid);
    }

    Widget::~Widget(){

        setScene->removeItem(item);
        setScene->clear();

        pixmap = QPixmap();
        pixmapFilt = QPixmap();
        image = QImage();

        delete setScene;
        delete viewImage;

        setScene = nullptr;
        viewImage = nullptr;
    }
