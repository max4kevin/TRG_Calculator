#include "calculationmapo.h"
#include "geometry.h"
#include <QGuiApplication>

//TODO: Replace call of "updateCalculation" method with signal-slot connections

CalculationMAPO::CalculationMAPO(IFrontendConnector& frontendConnector)
: CalculationBase (frontendConnector, "MAPO")
{
    Point tempPoint = {QPoint(0, 0), false, true, true, true, REGULAR_COLOR};

    workPoints_.append({points_.insert("S", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "S Point Description")});

    workPoints_.append({points_.insert("N", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "N Point Description")});

    workPoints_.append({points_.insert("A", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "A Point Description")});

    workPoints_.append({points_.insert("B", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "B Point Description")});

    workPoints_.append({points_.insert("Pog", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Pog Point Description")});

    workPoints_.append({points_.insert("C", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "C Point Description")});

    workPoints_.append({points_.insert("6", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "6 Point Description")});

    workPoints_.append({points_.insert("Se", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Se Point Description")});

    workPoints_.append({points_.insert("PNS", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "PNS Point Description")});

    workPoints_.append({points_.insert("ANS", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "ANS Point Description")});

    workPoints_.append({points_.insert("Go'", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Go' Point Description")});

    workPoints_.append({points_.insert("Ar", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Ar Point Description")});

    workPoints_.append({points_.insert("Rc", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Rc Point Description")});

    workPoints_.append({points_.insert("Is", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Is Point Description")});

    workPoints_.append({points_.insert("Ias", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Ias Point Description")});

    workPoints_.append({points_.insert("Ii", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Ii Point Description")});

    workPoints_.append({points_.insert("Iai", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Iai Point Description")});

    workPoints_.append({points_.insert("1", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "1 Point Description")});

    workPoints_.append({points_.insert("Me", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Me Point Description")});

    workPoints_.append({points_.insert("Go", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Go Point Description")});

    workPoints_.append({points_.insert("Gn", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Gn Point Description")});

    //Additional points

    tempPoint = {QPoint(0, 0), false, true, true, false, "#0000FF"};

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
    results_.append(resultsTable_.insert("SNA", {"", "82", QT_TRANSLATE_NOOP("Calculation", "SNA, deg")}));
    results_.append(resultsTable_.insert("SNB", {"", "80", QT_TRANSLATE_NOOP("Calculation", "SNB, deg")}));
    results_.append(resultsTable_.insert("ANB", {"", "2", QT_TRANSLATE_NOOP("Calculation", "ANB, deg")}));
    results_.append(resultsTable_.insert("SNPog", {"", "", QT_TRANSLATE_NOOP("Calculation", "SNPog, deg")})); //=SNB
    results_.append(resultsTable_.insert("Beta", {"", "27-35", QT_TRANSLATE_NOOP("Calculation", "Beta, deg")}));
    results_.append(resultsTable_.insert("Wits", {"", "1", QT_TRANSLATE_NOOP("Calculation", "Wits, mm")}));
    results_.append(resultsTable_.insert("Se-N", {"", "-", QT_TRANSLATE_NOOP("Calculation", "Se-N, mm")}));
    results_.append(resultsTable_.insert("A1-PNS", {"", "", QT_TRANSLATE_NOOP("Calculation", "A1-PNS, mm")})); //0.7*Se-N
    results_.append(resultsTable_.insert("Go-Gn", {"", "118 - 128", QT_TRANSLATE_NOOP("Calculation", "Go-Gn, mm")})); //Se-N+3(Se-N+6)
    results_.append(resultsTable_.insert("(S-Go)/(N-Me)", {"", "62-65", QT_TRANSLATE_NOOP("Calculation", "(S-Go)/(N-Me), %")}));
    results_.append(resultsTable_.insert("NSL-NL", {"", "8.5", QT_TRANSLATE_NOOP("Calculation", "NSL-NL, deg")}));
    results_.append(resultsTable_.insert("NSL-ML", {"", "32", QT_TRANSLATE_NOOP("Calculation", "NSL-ML, deg")}));
    results_.append(resultsTable_.insert("NL-ML", {"", "24", QT_TRANSLATE_NOOP("Calculation", "NL-ML, deg")}));
    results_.append(resultsTable_.insert("Go", {"", "130", QT_TRANSLATE_NOOP("Calculation", "Go, deg")}));
    results_.append(resultsTable_.insert("I-NSL", {"", "104", QT_TRANSLATE_NOOP("Calculation", "I-NSL, deg")}));
    results_.append(resultsTable_.insert("I-NL", {"", "110", QT_TRANSLATE_NOOP("Calculation", "I-NL, deg")}));
    results_.append(resultsTable_.insert("I-ML", {"", "95", QT_TRANSLATE_NOOP("Calculation", "I-ML, deg")}));
    results_.append(resultsTable_.insert("I-I", {"", "128", QT_TRANSLATE_NOOP("Calculation", "I-I, deg")}));
}

CalculationMAPO::~CalculationMAPO(){}

void CalculationMAPO::updateCalculation(const QString& lastPointName)
{
    checkCalibration(lastPointName);
    checkAngle(lastPointName, "SNA", "S", "N", "A");
    checkAngle(lastPointName, "SNB", "S", "N", "B");
    checkAngle(lastPointName, "ANB", "A", "N", "B");
    checkAngle(lastPointName, "SNPog", "S", "N", "Pog");
    checkPerpendicular(lastPointName, "pA/CB", "A", "C", "B");
    checkAngle(lastPointName, "Beta", "pA/CB", "A", "B");
    checkPerpendicular(lastPointName, "pA/16", "A", "6", "1");
    checkPerpendicular(lastPointName, "pB/16", "B", "6", "1");
    checkProjection(lastPointName, "Wits", "pB/16", "pA/16", "6", "1");
    checkDistance(lastPointName, "Se-N", "Se", "N");
    checkPerpendicular(lastPointName, "pA/PNSANS", "A", "PNS", "ANS");
    checkDistance(lastPointName, "A1-PNS", "PNS", "pA/PNSANS");
    checkDistance(lastPointName, "Go-Gn", "Go", "Gn");
    checkRatio(lastPointName, "(S-Go)/(N-Me)", "S", "Go", "N", "Me");
    checkCross(lastPointName, "NS/ANSPNS", "N", "S", "ANS", "PNS");
    checkAngle(lastPointName, "NSL-NL", "N", "NS/ANSPNS", "ANS");
    checkCross(lastPointName, "NS/MeGo", "N", "S", "Me", "Go");
    checkAngle(lastPointName, "NSL-ML", "N", "NS/MeGo", "Me");
    checkCross(lastPointName, "MeGo/ANSPNS", "Me", "Go", "ANS", "PNS");
    checkAngle(lastPointName, "NL-ML", "ANS", "MeGo/ANSPNS", "Me");
    checkCross(lastPointName, "ArRc/MeGo'", "Ar", "Rc", "Me", "Go'");
    checkAngle(lastPointName, "Go", "Ar", "ArRc/MeGo'", "Me");
    checkCross(lastPointName, "NS/IsIas", "N", "S", "Is", "Ias");
    checkAngle(lastPointName, "I-NSL", "S", "NS/IsIas", "Is");
    checkCross(lastPointName, "PNSANS/IsIas", "PNS", "ANS", "Is", "Ias");
    checkAngle(lastPointName, "I-NL", "PNS", "PNSANS/IsIas", "Is");
    checkCross(lastPointName, "IiIai/MeGo", "Ii", "Iai", "Me", "Go");
    checkAngle(lastPointName, "I-ML", "Ii", "IiIai/MeGo", "Go");
    checkCross(lastPointName, "IsIas/IiIai", "Ias", "Is", "Iai", "Ii");
    checkAngle(lastPointName, "I-I", "Ias", "IsIas/IiIai", "Iai");
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

void CalculationMAPO::checkAngle(const QString &checkPoint, const QString &dataName, const QString &pName1, const QString &pName2, const QString &pName3)
{
    CalculationBase::checkAngle(checkPoint, dataName, pName1, pName2, pName3);
    if (checkPoint == pName1 || checkPoint == pName2 || checkPoint == pName3)
    {
        if (dataName == "SNB")
        {
            resultsTable_["SNPog"].referenceValue = resultsTable_[dataName].value;
            updateResult("SNPog");
        }
    }
}

void CalculationMAPO::checkDistance(const QString &checkPoint, const QString &dataName, const QString &pName1, const QString &pName2)
{
    CalculationBase::checkDistance(checkPoint, dataName, pName1, pName2);
    if (checkPoint == pName1 || checkPoint == pName2)
    {
        if (dataName == "Se-N")
        {
            const QString &ref = resultsTable_[dataName].value;
            resultsTable_["A1-PNS"].referenceValue = (ref == "") ? ref : QString::number(0.7*ref.toFloat(), 'f', 2);
            updateResult("A1-PNS");
            resultsTable_["Go-Gn"].referenceValue = (ref == "") ?
                        ref : QString::number(ref.toFloat()+3, 'f', 2)+"("+QString::number(ref.toFloat()+6, 'f', 2)+")";
            updateResult("Go-Gn");
        }
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

//What perpendicular?
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
