#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <QLineF>

namespace geometry
{
    class Line
    {
        public:
            Line(qreal a, qreal b, qreal c);
            Line(QPointF p1, QPointF p2);
            Line(const Line& line);
            qreal a() const;
            qreal b() const;
            qreal c() const;

        private:
            qreal a_;
            qreal b_;
            qreal c_;
    };

    QPointF getCross(const geometry::Line& line1, const geometry::Line& line2);
    qreal calculateAngleRad(const QPointF& p1, const QPointF& p2, const QPointF& p3);
    qreal calculateAngleDeg(const QPointF& p1, const QPointF& p2, const QPointF& p3);
    QPointF getPerpendicularBase(const QPointF p, const geometry::Line& line);
    qreal calculateDistance(const QPointF p1, const QPointF p2);
    QPointF getBisectorBase(const QPointF& p1, const QPointF& p2, const QPointF& p3);
    QPointF getPerpendicularPoint(const QPointF p1, const QPointF p2);
    QPointF getMiddlePoint(const QPointF p1, const QPointF p2);
    qreal calculateProjection(const QPointF& p1, const QPointF& p2, const QPointF& p3, const QPointF& p4);
};

#endif // GEOMETRY_H
