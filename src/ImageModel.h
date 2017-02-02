#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

// Qt
#include <QList>
#include <QObject>
#include <QVariant>
#include <QVector2D>

class ImageModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal resolution READ resolution WRITE setResolution)
    Q_PROPERTY(QVariantList pixelData READ pixelData)

public:
    explicit ImageModel(QObject* parent = 0);
    virtual ~ImageModel();

    qreal resolution() const { return myResolution; }
    void setResolution(qreal resolution) { myResolution = resolution; }

    QVariantList pixelData() {
        QVariantList list;
        for (unsigned int i = 0; i < myPixelData.size(); ++i) {
            list.push_back( myPixelData[i] );
        }
        return list;
    }

    Q_INVOKABLE void rasterize(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3);

private:
    qreal myResolution;
    QList<QVector2D> myPixelData;

};

#endif // IMAGEMODEL_H
