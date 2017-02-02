#include "ImageModel.h"

typedef qreal Real;
typedef QVector2D Vector2;

ImageModel::ImageModel(QObject* parent) :
    QObject(parent),
    myPixelData(),
    myResolution(0.2)
{
}

ImageModel::~ImageModel()
{
}

/// determinant of triangle ABC
Real determinant(const Vector2 &a, const Vector2 &b, const Vector2 &c)
{
    return ( (b[0]-a[0])*(c[1]-a[1]) - (b[1]-a[1])*(c[0]-a[0]) );
}

/// area of triangle ABC
Real areaTriangle(const Vector2 &a, const Vector2 &b, const Vector2 &c) {
    return (determinant(a,b,c) / 2.0);
}

/// check if C is on left of AB
/// left if > 0 | collinear if == 0 | right if < 0
bool isLeft(const Vector2 &a, const Vector2 &b, const Vector2 &c)
{
    return (determinant(a, b, c) > 0);
}

/// check if C is on right of AB
/// left if > 0 | collinear if == 0 | right if < 0
bool isRight(const Vector2 &a, const Vector2 &b, const Vector2 &c)
{
    return (determinant(a, b, c) < 0);
}

/// Check if AB segment is a left edge (top-left rule from OpenGL and DirectX)
/// (assuming a third point C is on the left of AB)
bool isLeftEdge(const Vector2 &a, const Vector2 &b)
{
    return (b.y() != a.y() ? b.y() < a.y() : false);
}

/// Check if AB segment is a top edge (top-left rule from OpenGL and DirectX)
/// (assuming a third point C is on the left of AB)
bool isTopEdge(const Vector2 &a, const Vector2 &b)
{
    return (b.y() != a.y() ? false : b.x() > a.x());
}

/// Check if AB segment is a top or left edge (top-left rule from OpenGL and DirectX)
/// (assuming a third point C is on the left of AB)
bool isLeftorTopEdge(const Vector2 &a, const Vector2 &b) {
    return (isLeftEdge(a, b) || isTopEdge(a, b));
}

void addPixel(QList<Vector2>& pixelData, int x, int y)
{
    pixelData.push_back(Vector2(x,y));
}

void ImageModel::rasterize(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3)
{
    myPixelData.clear();

    Real epsilon = (Real) 0.0;

    float ri = myResolution;
    float rj = myResolution;

    Vector2 proj_p1(x1, y1);
    Vector2 proj_p2(x2, y2);
    Vector2 proj_p3(x3, y3);

    // if wrong orientation, switch two points
    if (isRight(proj_p1, proj_p2, proj_p3)) {
        Vector2 proj_tmp = proj_p2;
        proj_p2 = proj_p3;
        proj_p3 = proj_tmp;
    }

    // compute bounding box of current triangle
    Real triangle_minX = std::min( std::min(proj_p1[0], proj_p2[0]), proj_p3[0] );
    Real triangle_minY = std::min( std::min(proj_p1[1], proj_p2[1]), proj_p3[1] );
    Real triangle_maxX = std::max( std::max(proj_p1[0], proj_p2[0]), proj_p3[0] );
    Real triangle_maxY = std::max( std::max(proj_p1[1], proj_p2[1]), proj_p3[1] );

    // clip bounding box on pixel grid
    triangle_minX = std::floor(triangle_minX / ri) * ri;
    triangle_minY = std::floor(triangle_minY / rj) * rj;
    triangle_maxX = std::ceil(triangle_maxX / ri) * ri;
    triangle_maxY = std::ceil(triangle_maxY / rj) * rj;

    // dimension (in pixels) of bbox
    Vector2 dim((triangle_maxX - triangle_minX) / ri, (triangle_maxY - triangle_minY) / rj);

    // min position of camera (lower left bottom corner)
    Real camera_minX = -1;
    Real camera_minY = -1;

    // offset (in pixels) between camera bbox and triangle bbox
    unsigned int ti = std::round(( triangle_minX - camera_minX ) / ri);
    unsigned int tj = std::round(( triangle_minY - camera_minY ) / rj);

    for (unsigned int j = 0; j < dim[1]; ++j) {
        for (unsigned int i = 0; i < dim[0]; ++i) {
            // center of pixel (i,j)
            Real cx = triangle_minX + ((i + 0.5) * ri);
            Real cy = triangle_minY + ((j + 0.5) * rj);
            Vector2 P(cx, cy);

            // barycentrics coordinates
            Real area_p1 = determinant(proj_p2, proj_p3, P);
            Real area_p2 = determinant(proj_p3, proj_p1, P);
            Real area_p3 = determinant(proj_p1, proj_p2, P);

            if (area_p1 > epsilon && area_p2 > epsilon && area_p3 > epsilon) {
                addPixel(myPixelData, ti+i, tj+j);

            } else if (std::abs(area_p1) <= epsilon || std::abs(area_p2) <= epsilon || std::abs(area_p3) <= epsilon) {
                // test for edge/corner

                // one barycentric coordinate is equal to 0
                if (std::abs(area_p1) <= epsilon && area_p2 > epsilon && area_p3 > epsilon) {
                    // top-left rule on BC
                    if (isLeftorTopEdge(proj_p2, proj_p3)) {
                        addPixel(myPixelData, ti+i, tj+j);
                    }
                    continue;
                }
                if (area_p1 > epsilon && std::abs(area_p2) <= epsilon && area_p3 > epsilon) {
                    // top-left rule on CA
                    if (isLeftorTopEdge(proj_p3, proj_p1)) {
                        addPixel(myPixelData, ti+i, tj+j);
                    }
                    continue;
                }
                if (area_p1 > epsilon && area_p2 > epsilon && std::abs(area_p3) <= epsilon) {
                    // top-left rule on AB
                    if (isLeftorTopEdge(proj_p1, proj_p2)) {
                        addPixel(myPixelData, ti+i, tj+j);
                    }
                    continue;
                }

                // two barycentrics coordinates are equal to 0
                if (std::abs(area_p1) <= epsilon && std::abs(area_p2) <= epsilon && area_p3 > epsilon) {
                    // top-left rule on BC and CA
                    if (isLeftorTopEdge(proj_p2, proj_p3) && isLeftorTopEdge(proj_p3, proj_p1)) {
                        addPixel(myPixelData, ti+i, tj+j);
                    }
                    continue;
                }
                if (std::abs(area_p1) <= epsilon && area_p2 > epsilon && std::abs(area_p3) <= epsilon) {
                    // top-left rule on AB and BC
                    if (isLeftorTopEdge(proj_p1, proj_p2) && isLeftorTopEdge(proj_p2, proj_p3)) {
                        addPixel(myPixelData, ti+i, tj+j);
                    }
                    continue;
                }
                if (area_p1 > epsilon && std::abs(area_p2) <= epsilon && std::abs(area_p3) <= epsilon) {
                    // top-left rule on CA and AB
                    if (isLeftorTopEdge(proj_p3, proj_p1) && isLeftorTopEdge(proj_p1, proj_p2)) {
                        addPixel(myPixelData, ti+i, tj+j);
                    }
                    continue;
                }

                // three barycentrics coordinates are equal to 0, no pixel covered ...
                if (std::abs(area_p1) <= epsilon && std::abs(area_p2) <= epsilon && std::abs(area_p3) <= epsilon) {
                    continue;
                }
            }
        }
    }
}
