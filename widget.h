#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPushButton>




class ImageView : public QGraphicsView {
public:
    ImageView(QWidget* parent = nullptr) : QGraphicsView(parent) {}

protected:
    void resizeEvent(QResizeEvent* event) override {
        QGraphicsView::resizeEvent(event);
        fitInView(sceneRect(), Qt::KeepAspectRatio);
    }
};


class Widget : public QWidget
{
    Q_OBJECT

public slots:
    void CheckMem();

    void BlackInWhite();
    void LoadButtonImage();
    void ShowImageButton();
    void SaveImageButton();
    void LinearImageFiltering();

    void CreateButton(QPushButton *button, int width, int height);

    QPixmap ReturnImage(int unit);

    QImage ConvertToBlackAndWhite(const QImage& colorImage);
    QImage FilterImage3x3(QImage inputImage);


public:

    int number;
    bool accessView = nullptr;

    bool bolDefulat = false;// yes
    bool bolFilter = false;//n
    bool bolImageLoad = false;

    bool access = false;

    QString file_name;

    QPixmap pixmap; //imgs
    QPixmap pixmapFilt;
    QImage image; //_image

    QGraphicsScene* setScene = nullptr;
    ImageView* viewImage = nullptr;

    QGraphicsPixmapItem* item = nullptr;

    Widget(QWidget *parent = nullptr);

    ~Widget();
};
#endif // WIDGET_H
