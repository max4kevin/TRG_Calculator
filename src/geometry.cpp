#include "geometry.h"
#include <QtMath>
#include <QDebug>

//TODO: Zero value check?

geometry::Line::Line(qreal a, qreal b, qreal c)
    : a_(a), b_(b), c_(c) {}

geometry::Line::Line(QPointF p1, QPointF p2)
{
    a_ = p1.y() - p2.y();
    b_ = p2.x() - p1.x();
    c_ = p1.x()*p2.y() - p2.x()*p1.y();
}

geometry::Line::Line(const geometry::Line &line)
    : a_(line.a()), b_(line.b()), c_(line.c()) {}

qreal geometry::Line::a() const
{
    return a_;
}

qreal geometry::Line::b() const
{
    return b_;
}

qreal geometry::Line::c() const
{
    return c_;
}

//Cross of two lines
QPointF geometry::getCross(const geometry::Line& line1, const geometry::Line& line2)
{
    qreal x = (line1.b()*line2.c()-line2.b()*line1.c())/(line1.a()*line2.b()-line2.a()*line1.b());
    qreal y = (line2.a()*line1.c()-line1.a()*line2.c())/(line1.a()*line2.b()-line2.a()*line1.b());
    return QPointF(x, y);
}

//Angle from three points
qreal geometry::calculateAngleRad(const QPointF& p1, const QPointF& p2, const QPointF& p3)
{
    qreal aa = powf(p1.x()-p3.x(),2) + pow(p1.y()-p3.y(),2);
    qreal bb = powf(p1.x()-p2.x(),2) + pow(p1.y()-p2.y(),2);
    qreal cc = powf(p2.x()-p3.x(),2) + pow(p2.y()-p3.y(),2);
    return acos((bb+cc-aa)/(2*sqrt(bb)*sqrt(cc)));
}

qreal geometry::calculateAngleDeg(const QPointF &p1, const QPointF &p2, const QPointF &p3)
{
    return geometry::calculateAngleRad(p1, p2, p3)/M_PI*180;
}

//Perpendicular base point
QPointF geometry::getPerpendicularBase(const QPointF p, const geometry::Line& line)
{
    qreal x = (line.b()*(line.b()*p.x()-line.a()*p.y())-line.c()*line.a())/(powf(line.a(),2)+powf(line.b(),2));
    qreal y = (line.a()*(line.a()*p.y()-line.b()*p.x())-line.c()*line.b())/(powf(line.a(),2)+powf(line.b(),2));
    return QPointF(x, y);
}

qreal geometry::calculateDistance(const QPointF p1, const QPointF p2)
{
    return sqrt(powf(p1.x()-p2.x(),2) + powf(p1.y()-p2.y(),2));
}

geometry::Line getParallelLine(const geometry::Line& line, const QPointF& p)
{
    qreal c = - (line.a()*p.x() + line.b()*p.y());
    return geometry::Line(line.a(), line.b(), c);
}

QPointF geometry::getBisectorBase(const QPointF &p1, const QPointF &p2, const QPointF &p3)
{
    qreal p13 = geometry::calculateDistance(p1, p3);
    qreal p12 = geometry::calculateDistance(p1, p2);
    qreal p23 = geometry::calculateDistance(p2, p3);

    qreal a = 2*(p3.x() - p1.x());
    qreal b = 2*(p3.y() - p1.y());
    qreal c = pow(p13,2)*(pow(p23,2)-pow(p12,2))/pow(p23+p12,2) +
            pow(p1.x(),2) + pow(p1.y(),2) - pow(p3.x(),2) - pow(p3.y(),2);

    geometry::Line line1(a, b, c);
    geometry::Line line2(p1, p3);

    return geometry::getCross(line1, line2);
}

QPointF geometry::getPerpendicularPoint(const QPointF p1, const QPointF p2)
{
    qreal d = geometry::calculateDistance(p1, p2);
    qreal rr = 1.25*pow(d,2);
    qreal D = 0.25*sqrt(pow(d,2)*(4*rr-pow(d,2)));
    qreal x = (p1.x()+p2.x())/2 - 2*(p1.y()-p2.y())*D/pow(d,2);
    qreal y = (p1.y()+p2.y())/2 + 2*(p1.x()-p2.x())*D/pow(d,2);
    return QPointF(x,y);
}

QPointF geometry::getMiddlePoint(const QPointF p1, const QPointF p2)
{
    qreal x = (p1.x()+p2.x())/2;
    qreal y = (p1.y()+p2.y())/2;
    return QPointF(x,y);
}

qreal geometry::calculateProjection(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4)
{
    qreal length = geometry::calculateDistance(p3, p4);
    return ((p2.x()-p1.x())*(p4.x()-p3.x()) + (p2.y()-p1.y())*(p4.y()-p3.y()))/length;
}
