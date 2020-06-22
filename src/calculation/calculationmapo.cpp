#include "calculationmapo.h"
#include "geometry.h"

CalculationMAPO::CalculationMAPO(IFrontendConnector& frontendConnector)
: CalculationBase (frontendConnector)
{
    Point tempPoint = {QPoint(0, 0), false, true, true, true, REGULAR_COLOR, ""};

    workPoints_.append(points_.insert("S", tempPoint));
    workPoints_.append(points_.insert("N", tempPoint));
    workPoints_.append(points_.insert("A", tempPoint));
    workPoints_.append(points_.insert("B", tempPoint));
    workPoints_.append(points_.insert("Pog", tempPoint));
    workPoints_.append(points_.insert("C", tempPoint));
    workPoints_.append(points_.insert("6", tempPoint));
    workPoints_.append(points_.insert("Se", tempPoint));
    workPoints_.append(points_.insert("PNS", tempPoint));
    workPoints_.append(points_.insert("ANS", tempPoint));
    workPoints_.append(points_.insert("Go'", tempPoint));
    workPoints_.append(points_.insert("Ar", tempPoint));
    workPoints_.append(points_.insert("Rc", tempPoint));
    workPoints_.append(points_.insert("Is", tempPoint));
    workPoints_.append(points_.insert("Ias", tempPoint));
    workPoints_.append(points_.insert("Ii", tempPoint));
    workPoints_.append(points_.insert("Iai", tempPoint));
    workPoints_.append(points_.insert("1", tempPoint));
    workPoints_.append(points_.insert("Me", tempPoint));
    workPoints_.append(points_.insert("Go", tempPoint));
    workPoints_.append(points_.insert("Gn", tempPoint));

    //Additional points

    tempPoint = {QPoint(0, 0), false, true, true, false, "#0000FF", ""};

    points_.insert("NS/ANSPNS", tempPoint);
    points_.insert("NS/MeGo", tempPoint);
    points_.insert("MeGo/ANSPNS", tempPoint);
    points_.insert("ArRc/MeGo'", tempPoint);
    points_.insert("NS/IsIas", tempPoint);
    points_.insert("PNSANS/IsIas", tempPoint);
    points_.insert("IiIai/MeGo", tempPoint);
    points_.insert("IsIas/IiIai", tempPoint);

    points_.insert("pA/CB", tempPoint);
    points_.insert("pA/16", tempPoint);
    points_.insert("pB/16", tempPoint);
    points_.insert("pA/PNSANS", tempPoint);

    points_.insert("BisArGoMe", tempPoint);

    points_.insert("MidPogMe", tempPoint);
    points_.insert("PenPogMe", tempPoint);

    //Results
    results_.append(resultsTable_.insert("SNA, deg", {"", "82"}));
    results_.append(resultsTable_.insert("SNB, deg", {"", "80"}));
    results_.append(resultsTable_.insert("ANB, deg", {"", "2"}));
    results_.append(resultsTable_.insert("SNPog, deg", {"", ""})); //=SNB
    results_.append(resultsTable_.insert("Beta, deg", {"", "27-35"}));
    results_.append(resultsTable_.insert("Wits, mm", {"", "1"}));
    results_.append(resultsTable_.insert("Se-N, mm", {"", "-"}));
    results_.append(resultsTable_.insert("A1-PNS, mm", {"", ""})); //0.7*Se-N
    results_.append(resultsTable_.insert("Go-Gn, mm", {"", "118 - 128"})); //Se-N+3(Se-N+6)
    results_.append(resultsTable_.insert("(S-Go)/(N-Me), %", {"", "62-65"}));
    results_.append(resultsTable_.insert("NSL-NL, deg", {"", "8.5"}));
    results_.append(resultsTable_.insert("NSL-ML, deg", {"", "32"}));
    results_.append(resultsTable_.insert("NL-ML, deg", {"", "24"}));
    results_.append(resultsTable_.insert("Go, deg", {"", "130"}));
    results_.append(resultsTable_.insert("I-NSL, deg", {"", "104"}));
    results_.append(resultsTable_.insert("I-NL, deg", {"", "110"}));
    results_.append(resultsTable_.insert("I-ML, deg", {"", "95"}));
    results_.append(resultsTable_.insert("I-I, deg", {"", "128"}));
}

CalculationMAPO::~CalculationMAPO(){}

void CalculationMAPO::updateCalculation(const QString& lastPointName)
{
    checkCalibration(lastPointName);
    checkAngle(lastPointName, "SNA, deg", "S", "N", "A");
    checkAngle(lastPointName, "SNB, deg", "S", "N", "B");
    checkAngle(lastPointName, "ANB, deg", "A", "N", "B");
    checkAngle(lastPointName, "SNPog, deg", "S", "N", "Pog");
    checkPerpendicular(lastPointName, "pA/CB", "A", "C", "B");
    checkAngle(lastPointName, "Beta, deg", "pA/CB", "A", "B");
    checkPerpendicular(lastPointName, "pA/16", "A", "6", "1");
    checkPerpendicular(lastPointName, "pB/16", "B", "6", "1");
    checkProjection(lastPointName, "Wits, mm", "pB/16", "pA/16", "6", "1");
    checkDistance(lastPointName, "Se-N, mm", "Se", "N");
    checkPerpendicular(lastPointName, "pA/PNSANS", "A", "PNS", "ANS");
    checkDistance(lastPointName, "A1-PNS, mm", "PNS", "pA/PNSANS");
    checkDistance(lastPointName, "Go-Gn, mm", "Go", "Gn");
    checkRatio(lastPointName, "(S-Go)/(N-Me), %", "S", "Go", "N", "Me");
    checkCross(lastPointName, "NS/ANSPNS", "N", "S", "ANS", "PNS");
    checkAngle(lastPointName, "NSL-NL, deg", "N", "NS/ANSPNS", "ANS");
    checkCross(lastPointName, "NS/MeGo", "N", "S", "Me", "Go");
    checkAngle(lastPointName, "NSL-ML, deg", "N", "NS/MeGo", "Me");
    checkCross(lastPointName, "MeGo/ANSPNS", "Me", "Go", "ANS", "PNS");
    checkAngle(lastPointName, "NL-ML, deg", "ANS", "MeGo/ANSPNS", "Me");
    checkCross(lastPointName, "ArRc/MeGo'", "Ar", "Rc", "Me", "Go'");
    checkAngle(lastPointName, "Go, deg", "Ar", "ArRc/MeGo'", "Me");
    checkCross(lastPointName, "NS/IsIas", "N", "S", "Is", "Ias");
    checkAngle(lastPointName, "I-NSL, deg", "S", "NS/IsIas", "Is");
    checkCross(lastPointName, "PNSANS/IsIas", "PNS", "ANS", "Is", "Ias");
    checkAngle(lastPointName, "I-NL, deg", "PNS", "PNSANS/IsIas", "Is");
    checkCross(lastPointName, "IiIai/MeGo", "Ii", "Iai", "Me", "Go");
    checkAngle(lastPointName, "I-ML, deg", "Ii", "IiIai/MeGo", "Go");
    checkCross(lastPointName, "IsIas/IiIai", "Ias", "Is", "Iai", "Ii");
    checkAngle(lastPointName, "I-I, deg", "Ias", "IsIas/IiIai", "Iai");
    checkBisect(lastPointName, "BisArGoMe", "Ar", "ArRc/MeGo'", "Me");
    checkMiddlePoint(lastPointName, "1", "Is", "Ii");
    checkMiddlePoint(lastPointName, "MidPogMe", "Pog", "Me");
    checkAddPerpendicular(lastPointName, "PenPogMe", "Pog", "Me");

    checkPointsConnection(lastPointName, "S", "N");
    checkPointsConnection(lastPointName, "N", "A");
    checkPointsConnection(lastPointName, "N", "B");
    checkPointsConnection(lastPointName, "N", "Pog");
    checkPointsConnection(lastPointName, "A", "pA/CB");
    checkPointsConnection(lastPointName, "6", "1");
    checkPointsConnection(lastPointName, "C", "B");
    checkPointsConnection(lastPointName, "A", "B");
    checkPointsConnection(lastPointName, "A", "pA/16");
    checkPointsConnection(lastPointName, "B", "pB/16");
    checkPointsConnection(lastPointName, "pA/16", "pB/16");
    checkPointsConnection(lastPointName, "Se", "N");
    checkPointsConnection(lastPointName, "A", "pA/PNSANS");
    checkPointsConnection(lastPointName, "PNS", "pA/PNSANS");
    checkPointsConnection(lastPointName, "Go", "Gn");
    checkPointsConnection(lastPointName, "S", "Go");
    checkPointsConnection(lastPointName, "N", "Me");
    checkPointsConnection(lastPointName, "N", "NS/ANSPNS");
    checkPointsConnection(lastPointName, "NS/ANSPNS", "ANS");
    checkPointsConnection(lastPointName, "N", "NS/MeGo");
    checkPointsConnection(lastPointName, "NS/MeGo", "Me");
    checkPointsConnection(lastPointName, "ANS", "MeGo/ANSPNS");
    checkPointsConnection(lastPointName, "MeGo/ANSPNS", "Me");
    checkPointsConnection(lastPointName, "Ar", "ArRc/MeGo'");
    checkPointsConnection(lastPointName, "ArRc/MeGo'", "Me");
    checkPointsConnection(lastPointName, "S", "NS/IsIas");
    checkPointsConnection(lastPointName, "NS/IsIas", "Is");
    checkPointsConnection(lastPointName, "PNS", "PNSANS/IsIas");
    checkPointsConnection(lastPointName, "PNSANS/IsIas", "Is");
    checkPointsConnection(lastPointName, "Ii", "IiIai/MeGo");
    checkPointsConnection(lastPointName, "IiIai/MeGo", "Go");
    checkPointsConnection(lastPointName, "Ias", "IsIas/IiIai");
    checkPointsConnection(lastPointName, "IsIas/IiIai", "Iai");
    checkPointsConnection(lastPointName, "ArRc/MeGo'", "BisArGoMe");
    checkPointsConnection(lastPointName, "PenPogMe", "MidPogMe");
}

void CalculationMAPO::checkAngle(const QString& checkPoint, const QString &dataName, const QString &pName1, const QString &pName2, const QString &pName3)
{
    if (checkPoint == pName1 || checkPoint == pName2 || checkPoint == pName3)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady && points_[pName3].isReady )
        {
            resultsTable_[dataName].value =
                    QString::number(geometry::calculateAngleDeg(points_[pName1].coordinates, points_[pName2].coordinates, points_[pName3].coordinates), 'f', 2);
        }
        else
        {
            resultsTable_[dataName].value = "";
        }
        updateResult(dataName);
        if (dataName == "SNB, deg")
        {
            resultsTable_["SNPog, deg"].referenceValue = resultsTable_[dataName].value;
            updateResult("SNPog, deg");
        }
    }
}

void CalculationMAPO::checkDistance(const QString& checkPoint, const QString &dataName, const QString &pName1, const QString &pName2)
{
    if (checkPoint == pName1 || checkPoint == pName2)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady )
        {
            resultsTable_[dataName].value =
                    QString::number(geometry::calculateDistance(points_[pName1].coordinates, points_[pName2].coordinates)*getCalibrationValue(), 'f', 2);
        }
        else
        {
            resultsTable_[dataName].value = "";
        }
        updateResult(dataName);

        if (dataName == "Se-N, mm")
        {
            const QString &ref = resultsTable_[dataName].value;
            resultsTable_["A1-PNS, mm"].referenceValue = (ref == "") ? ref : QString::number(0.7*ref.toFloat(), 'f', 2);
            updateResult("A1-PNS, mm");
            resultsTable_["Go-Gn, mm"].referenceValue = (ref == "") ?
                        ref : QString::number(ref.toFloat()+3, 'f', 2)+"("+QString::number(ref.toFloat()+6, 'f', 2)+")";
            updateResult("Go-Gn, mm");
        }
    }
}

void CalculationMAPO::checkRatio(const QString& checkPoint, const QString &dataName, const QString &pName1, const QString &pName2, const QString &pName3, const QString &pName4)
{
    if (checkPoint == pName1 || checkPoint == pName2 || checkPoint == pName3 || checkPoint == pName4)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady && points_[pName3].isReady && points_[pName4].isReady)
        {
            qreal dist1 = geometry::calculateDistance(points_[pName1].coordinates, points_[pName2].coordinates);
            qreal dist2 = geometry::calculateDistance(points_[pName3].coordinates, points_[pName4].coordinates);
            resultsTable_[dataName].value = QString::number(dist1/dist2*100, 'f', 2);
        }
        else
        {
            resultsTable_[dataName].value = "";
        }
        updateResult(dataName);
    }
}

void CalculationMAPO::checkProjection(const QString& checkPoint, const QString &dataName, const QString &pName1, const QString &pName2, const QString &pName3, const QString &pName4)
{
    if (checkPoint == pName1 || checkPoint == pName2 || checkPoint == pName3 || checkPoint == pName4)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady && points_[pName3].isReady && points_[pName4].isReady)
        {
            resultsTable_[dataName].value =
                    QString::number(geometry::calculateProjection(points_[pName1].coordinates, points_[pName2].coordinates,
                                                                  points_[pName3].coordinates, points_[pName4].coordinates)*getCalibrationValue(), 'f', 2);
        }
        else
        {
            resultsTable_[dataName].value = "";
        }
        updateResult(dataName);
    }
}

void CalculationMAPO::checkCross(const QString& checkPoint, const QString &pointName, const QString &pName1, const QString &pName2, const QString &pName3, const QString &pName4)
{
    if (checkPoint == pName1 || checkPoint == pName2 || checkPoint == pName3 || checkPoint == pName4)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady && points_[pName3].isReady && points_[pName4].isReady )
        {
            geometry::Line line1(points_[pName1].coordinates, points_[pName2].coordinates);
            geometry::Line line2(points_[pName3].coordinates, points_[pName4].coordinates);
            points_[pointName].coordinates = geometry::getCross(line1, line2);
            points_[pointName].isReady = true;
        }
        else
        {
            points_[pointName].isReady = false;
        }
        updatePoint(pointName);
        updateCalculation(pointName);
    }
}

void CalculationMAPO::checkPerpendicular(const QString& checkPoint, const QString &pointName, const QString &pName0, const QString &pName1, const QString &pName2)
{
    if (checkPoint == pName1 || checkPoint == pName2)
    {
        if ( points_[pName0].isReady && points_[pName1].isReady && points_[pName2].isReady )
        {
            geometry::Line line1(points_[pName1].coordinates, points_[pName2].coordinates);
            points_[pointName].coordinates = geometry::getPerpendicularBase(points_[pName0].coordinates, line1);
            points_[pointName].isReady = true;
        }
        else
        {
            points_[pointName].isReady = false;
        }
        updatePoint(pointName);
        updateCalculation(pointName);
    }
}

void CalculationMAPO::checkBisect(const QString& checkPoint, const QString &pointName, const QString &pName1, const QString &pName2, const QString &pName3)
{
    if (checkPoint == pName1 || checkPoint == pName2 || checkPoint == pName3)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady && points_[pName3].isReady )
        {
            points_[pointName].coordinates = geometry::getBisectorBase(points_[pName1].coordinates, points_[pName2].coordinates, points_[pName3].coordinates);
            points_[pointName].isReady = true;
        }
        else
        {
            points_[pointName].isReady = false;
        }
        updatePoint(pointName);
        updateCalculation(pointName);
    }
}

void CalculationMAPO::checkMiddlePoint(const QString& checkPoint, const QString &pointName, const QString &pName1, const QString &pName2)
{
    if (checkPoint == pName1 || checkPoint == pName2)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady)
        {
            points_[pointName].coordinates = geometry::getMiddlePoint(points_[pName1].coordinates, points_[pName2].coordinates);
            points_[pointName].isReady = true;
        }
        else
        {
            points_[pointName].isReady = false;
        }
        updatePoint(pointName);
        updateCalculation(pointName);
    }
}

void CalculationMAPO::checkAddPerpendicular(const QString& checkPoint, const QString &pointName, const QString &pName1, const QString &pName2)
{
    if (checkPoint == pName1 || checkPoint == pName2)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady)
        {
            points_[pointName].coordinates = geometry::getPerpendicularPoint(points_[pName1].coordinates, points_[pName2].coordinates);
            points_[pointName].isReady = true;
        }
        else
        {
            points_[pointName].isReady = false;
        }
        updatePoint(pointName);
        updateCalculation(pointName);
    }
}
