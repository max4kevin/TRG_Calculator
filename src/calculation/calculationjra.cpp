#include "calculationjra.h"

CalculationJRA::CalculationJRA(IFrontendConnector& frontendConnector)
: CalculationBase (frontendConnector, "JRA")
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

    workPoints_.append({points_.insert("Ba", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Ba Point Description")});

    workPoints_.append({points_.insert("Pt", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Pt Point Description")});

    workPoints_.append({points_.insert("Po", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Po Point Description")});

    workPoints_.append({points_.insert("Or", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Or Point Description")});

    workPoints_.append({points_.insert("Ar", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Ar Point Description")});

    workPoints_.append({points_.insert("ANS", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "ANS Point Description")});

    workPoints_.append({points_.insert("PNS", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "PNS Point Description")});

    workPoints_.append({points_.insert("PMi", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "PMi Point Description")});

    workPoints_.append({points_.insert("PMp", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "PMp Point Description")});

    workPoints_.append({points_.insert("Pog", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Pog Point Description")});

    workPoints_.append({points_.insert("Me", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Me Point Description")});

    workPoints_.append({points_.insert("Is", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Is Point Description")});

    workPoints_.append({points_.insert("Ii", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Ii Point Description")});

    workPoints_.append({points_.insert("ApIs", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "ApIs Point Description")});

    workPoints_.append({points_.insert("ApIi", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "ApIi Point Description")});

    workPoints_.append({points_.insert("OcPM", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "OcPM Point Description")});

    workPoints_.append({points_.insert("OcPPr", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "OcPPr Point Description")});

    workPoints_.append({points_.insert("OcPI", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "OcPI Point Description")});

    workPoints_.append({points_.insert("StS", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "StS Point Description")});

    workPoints_.append({points_.insert("Li", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Li Point Description")});

    workPoints_.append({points_.insert("Prn", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Prn Point Description")});

    workPoints_.append({points_.insert("Pog'", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Pog' Point Description")});

    workPoints_.append({points_.insert("Gn", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Gn Point Description")});

    workPoints_.append({points_.insert("Go", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Go Point Description")});

    //Additional points

    tempPoint = {QPoint(0, 0), false, true, true, false, "#0000FF"};

    points_.insert("ApIsIs/IiApIi", tempPoint);
    points_.insert("APog/IiApIi", tempPoint);
    points_.insert("APog/IsApIs", tempPoint);
    points_.insert("OrPo/OcPMOcPI", tempPoint);
    points_.insert("PoOr/NA", tempPoint);
    points_.insert("ArPMp/PMiMe", tempPoint);
    points_.insert("ArPMp/PMiMeMe/PogN", tempPoint);
    points_.insert("BaN/PtGn", tempPoint);
    points_.insert("OrPo/PMiMe", tempPoint);
    points_.insert("ANSPNS/GoGn", tempPoint);

    points_.insert("pIs/APog", tempPoint);
    points_.insert("pIi/APog", tempPoint);
    points_.insert("pStS/OcPMOcPPr", tempPoint);
    points_.insert("pLi/PrnPog'", tempPoint);
    points_.insert("pA/NPog", tempPoint);
    points_.insert("pLi/PrnPog'", tempPoint);
    points_.insert("pA/PrnPog'", tempPoint);

    points_.insert("BisArGoMe", tempPoint);
    points_.insert("MidPogMe", tempPoint);
    points_.insert("PenPogMe", tempPoint);

    //Results
    results_.append(resultsTable_.insert("DentEst_Group", {"", "",
    QT_TRANSLATE_NOOP("Calculation", "----DENTAL AND ESTHETICS----")}));

    results_.append(resultsTable_.insert("U1-APo(mm)", {"", "6.0±2.2",
    QT_TRANSLATE_NOOP("Calculation", "U-Insisors Protrusion (U1-APo), mm")}));

    results_.append(resultsTable_.insert("L1-APo(mm)", {"", "2.7±1.7",
    QT_TRANSLATE_NOOP("Calculation", "L1 Protrusion (L1-APo), mm")}));

    results_.append(resultsTable_.insert("U1-L1(deg)", {"", "130.0±6.0",
    QT_TRANSLATE_NOOP("Calculation", "Interincisal Angle (U1-L1), deg")}));

    results_.append(resultsTable_.insert("U1-APo(deg)", {"", "28.0±4.0",
    QT_TRANSLATE_NOOP("Calculation", "U-Insisors Inclination (U1-APo), deg")}));

    results_.append(resultsTable_.insert("L1-APo(deg)", {"", "22.0±4.0",
    QT_TRANSLATE_NOOP("Calculation", "L1 to A-Po, deg")}));

    results_.append(resultsTable_.insert("OccPlaneFH(deg)", {"", "6.8±5.0",
    QT_TRANSLATE_NOOP("Calculation", "Occ Plane to FH, deg")}));

    results_.append(resultsTable_.insert("Stm-FOP(mm)", {"", "4.5±2.0",
    QT_TRANSLATE_NOOP("Calculation", "Commeasure Height (Stm-FOP), mm")}));

    results_.append(resultsTable_.insert("LowerLipE-plane", {"", "-2.0±2.0",
    QT_TRANSLATE_NOOP("Calculation", "Lower Lip to E-plane, mm")}));



    results_.append(resultsTable_.insert("Skeletal_Group", {"", "",
    QT_TRANSLATE_NOOP("Calculation", "----SKELETAL----")}));

    //add result

    results_.append(resultsTable_.insert("A-NPo(mm)", {"", "0.7±2.0",
    QT_TRANSLATE_NOOP("Calculation", "Convexity (A-NPo), mm")}));

    results_.append(resultsTable_.insert("FH-NA(deg)", {"", "90.0±3.0",
    QT_TRANSLATE_NOOP("Calculation", "Maxillary Depth (FH-NA), deg")}));

    results_.append(resultsTable_.insert("FacialTaper", {"", "68.0±3.5",
    QT_TRANSLATE_NOOP("Calculation", "Facial Taper, deg")}));

    results_.append(resultsTable_.insert("NaBa-PtGn(deg)", {"", "90.0±3.5",
    QT_TRANSLATE_NOOP("Calculation", "Facial Axis-Ricketts (NaBa-PtGn), deg")}));

    results_.append(resultsTable_.insert("MP-FH(deg)", {"", "23.9±4.5",
    QT_TRANSLATE_NOOP("Calculation", "FMA (MP-FH), deg")}));

    results_.append(resultsTable_.insert("Ar-Go-Me(deg)", {"", "122.9±6.7",
    QT_TRANSLATE_NOOP("Calculation", "Gonial/Jaw Angle (Ar-Go-Me), deg")}));

    results_.append(resultsTable_.insert("PP-MP(deg)", {"", "25.0±6.0",
    QT_TRANSLATE_NOOP("Calculation", "Palatal-Mand Angle (PP-MP), deg")}));



    results_.append(resultsTable_.insert("Growth1_Group", {"", "",
    QT_TRANSLATE_NOOP("Calculation", "----GROWTH----\n[= Meso, < Brachy, > Dolicho]")}));

    results_.append(resultsTable_.insert("SN-Ar(deg)", {"", "124.0±5.0",
    QT_TRANSLATE_NOOP("Calculation", "Saddle/Sella Angle (SN-Ar), deg")}));

    results_.append(resultsTable_.insert("ArticularAngle", {"", "140.3±6.0",
    QT_TRANSLATE_NOOP("Calculation", "Articular Angle, deg")}));

    results_.append(resultsTable_.insert("Ar-Go-Me2(deg)", {"", "122.9±6.7",
    QT_TRANSLATE_NOOP("Calculation", "Gonial/Jaw Angle (Ar-Go-Me), deg")})); //Attention: Dublicate

    results_.append(resultsTable_.insert("Jarabak", {"", "386.6±6.0",
    QT_TRANSLATE_NOOP("Calculation", "Sum of Angles (Jarabak), deg")}));



    results_.append(resultsTable_.insert("Growth2_Group", {"", "",
    QT_TRANSLATE_NOOP("Calculation", "----GROWTH----\n[UGA < Horz tendency; LGA > Vert tendency]")}));

    results_.append(resultsTable_.insert("Ar-Go-Na(deg)", {"", "52.0±7.0",
    QT_TRANSLATE_NOOP("Calculation", "Upper Gonial Angle (Ar-Go-Na), deg")}));

    results_.append(resultsTable_.insert("Na-Go-Na(deg)", {"", "71.2±6.0",
    QT_TRANSLATE_NOOP("Calculation", "Lower Gonial Angle (Na-Go-Na), deg")}));



    results_.append(resultsTable_.insert("Mandibular_Group", {"", "",
    QT_TRANSLATE_NOOP("Calculation", "----MANDIBULAR BODY GROWTH RATE----\n[= Normal, < Diminished, > Augmented]")}));

    results_.append(resultsTable_.insert("SN(mm)", {"", "75.3±3.0",
    QT_TRANSLATE_NOOP("Calculation", "Anterior Cranial Base (SN), mm")}));

    results_.append(resultsTable_.insert("Go-Gn(mm)", {"", "75.2±4.4",
    QT_TRANSLATE_NOOP("Calculation", "Mandibular Body Length (Go-Gn), mm")}));

    results_.append(resultsTable_.insert("JarabakAnterior", {"", "93.0±4.0",
    QT_TRANSLATE_NOOP("Calculation", "Jarabak Anterior Ratio (x100)")}));



    results_.append(resultsTable_.insert("Ramus_Group", {"", "",
    QT_TRANSLATE_NOOP("Calculation", "----RAMUS GROWTH RATE----\n[= Normal, < Diminished, > Augmented]")}));

    results_.append(resultsTable_.insert("SAr(mm)", {"", "35.0±4.0",
    QT_TRANSLATE_NOOP("Calculation", "Posterior Cranial Base (SAr), mm")}));

    results_.append(resultsTable_.insert("Ar-Go(mm)", {"", "48.5±4.5",
    QT_TRANSLATE_NOOP("Calculation", "Ramus Height (Ar-Go), mm")}));

    results_.append(resultsTable_.insert("S-Ar/Ar-Go", {"", "75.0±5.0",
    QT_TRANSLATE_NOOP("Calculation", "S-Ar/Ar-Go, %")}));



    results_.append(resultsTable_.insert("Growth3_Group", {"", "",
    QT_TRANSLATE_NOOP("Calculation", "----GROWTH----\n[= Normal, < Divergent, > Convergent]")}));

    results_.append(resultsTable_.insert("NaMe(mm)", {"", "128.5±5.0",
    QT_TRANSLATE_NOOP("Calculation", "Anterior Face Height (NaMe), mm")}));

    results_.append(resultsTable_.insert("SGo(mm)", {"", "82.5±5.0",
    QT_TRANSLATE_NOOP("Calculation", "Posterior Face Height (SGo), mm")}));

    results_.append(resultsTable_.insert("S-Go/Na-Me", {"", "65.0±4.0",
    QT_TRANSLATE_NOOP("Calculation", "P-A Face Height (S-Go/Na-Me), %")}));



    results_.append(resultsTable_.insert("LowerFacial_Group", {"", "",
    QT_TRANSLATE_NOOP("Calculation", "----LOWER 1/3 FACIAL VERTICAL PROBLEMS----\n[Yes/No]")}));

    results_.append(resultsTable_.insert("ANS-Me(mm)", {"", "71.5±5.0",
    QT_TRANSLATE_NOOP("Calculation", "Anterior Facial Ht (ANS-Me), mm")}));

    results_.append(resultsTable_.insert("ANS-Me/Na-Me", {"", "55.0±0.1",
    QT_TRANSLATE_NOOP("Calculation", "ANS-Me/Na-Me, %")}));
}

CalculationJRA::~CalculationJRA() {}

void CalculationJRA::updateCalculation(const QString &lastPointName)
{
    checkCalibration(lastPointName);
    checkPerpendicular(lastPointName, "pIs/APog", "Is", "A", "Pog");
    checkDistance(lastPointName, "U1-APo(mm)", "Is", "pIs/APog");
    checkPerpendicular(lastPointName, "pIi/APog", "Ii", "A", "Pog");
    checkDistance(lastPointName, "L1-APo(mm)", "Ii", "pIi/APog");
    checkCross(lastPointName, "ApIsIs/IiApIi", "ApIs", "Is", "Ii", "ApIi");
    checkAngle(lastPointName, "U1-L1(deg)", "ApIs", "ApIsIs/IiApIi", "ApIi");
    checkCross(lastPointName, "APog/IsApIs", "A", "Pog", "Is", "ApIs");
    checkAngle(lastPointName, "U1-APo(deg)", "A", "APog/IsApIs", "ApIs");
    checkCross(lastPointName, "APog/IiApIi", "A", "Pog", "Ii", "ApIi");
    checkAngle(lastPointName, "L1-APo(deg)", "A", "APog/IiApIi", "Ii");
    checkCross(lastPointName, "OrPo/OcPMOcPI", "Or", "Po", "OcPM", "OcPI");
    checkAngle(lastPointName, "OccPlaneFH(deg)", "Or", "OrPo/OcPMOcPI", "OcPI");
    checkPerpendicular(lastPointName, "pStS/OcPMOcPPr", "StS", "OcPM", "OcPPr");
    checkDistance(lastPointName, "Stm-FOP(mm)", "StS", "pStS/OcPMOcPPr");
    checkPerpendicular(lastPointName, "pLi/PrnPog'", "Li", "Prn", "Pog'");
    checkPerpendicular(lastPointName, "pA/PrnPog'", "A", "Prn", "Pog'");
    checkProjection(lastPointName, "LowerLipE-plane", "Li", "pLi/PrnPog'", "pA/PrnPog'", "A");

    checkPerpendicular(lastPointName, "pA/NPog", "A", "N", "Pog");
    checkDistance(lastPointName, "A-NPo(mm)", "A", "pA/NPog");
    checkCross(lastPointName, "PoOr/NA", "Po", "Or", "N", "A");
    checkAngle(lastPointName, "FH-NA(deg)", "Po", "PoOr/NA", "A");
    checkCross(lastPointName, "ArPMp/PMiMeMe/PogN", "ArPMp/PMiMe", "Me", "Pog", "N");
    checkAngle(lastPointName, "FacialTaper", "ArPMp/PMiMe", "ArPMp/PMiMeMe/PogN", "N");
    checkCross(lastPointName, "BaN/PtGn", "Ba", "N", "Pt", "Gn");
    checkAngle(lastPointName, "NaBa-PtGn(deg)", "Ba", "BaN/PtGn", "Gn");
    checkCross(lastPointName, "OrPo/PMiMe", "Or", "Po", "PMi", "Me");
    checkAngle(lastPointName, "MP-FH(deg)", "Or", "OrPo/PMiMe", "Me");
    checkCross(lastPointName, "ArPMp/PMiMe", "Ar", "PMp", "PMi", "Me");
    checkAngle(lastPointName, "Ar-Go-Me(deg)", "Ar", "ArPMp/PMiMe", "Me");
    checkCross(lastPointName, "ANSPNS/GoGn", "ANS", "PNS", "Go", "Gn");
    checkAngle(lastPointName, "PP-MP(deg)", "ANS", "ANSPNS/GoGn", "Gn");

    checkAngle(lastPointName, "SN-Ar(deg)", "N", "S", "Ar");
    checkAngle(lastPointName, "ArticularAngle", "S", "Ar", "Go");
    checkAngle(lastPointName, "Ar-Go-Me2(deg)", "Ar", "ArPMp/PMiMe", "Me");

    checkAngle(lastPointName, "Ar-Go-Na(deg)", "Ar", "ArPMp/PMiMe", "N");
    checkAngle(lastPointName, "Na-Go-Na(deg)", "N", "ArPMp/PMiMe", "Me");

    checkDistance(lastPointName, "SN(mm)", "S", "N");
    checkDistance(lastPointName, "Go-Gn(mm)", "Go", "Gn");
    checkRatio(lastPointName, "JarabakAnterior", "S", "N", "Go", "Gn");

    checkDistance(lastPointName, "SAr(mm)", "S", "Ar");
    checkDistance(lastPointName, "Ar-Go(mm)", "Ar", "Go");
    checkRatio(lastPointName, "S-Ar/Ar-Go", "S", "Ar", "Ar", "Go");

    checkDistance(lastPointName, "NaMe(mm)", "N", "Me");
    checkDistance(lastPointName, "SGo(mm)", "S", "Go");
    checkRatio(lastPointName, "S-Go/Na-Me", "S", "Go", "N", "Me");

    checkDistance(lastPointName, "ANS-Me(mm)", "ANS", "Me");
    checkRatio(lastPointName, "ANS-Me/Na-Me", "ANS", "Me", "N", "Me");
    checkMiddlePoint(lastPointName, "OcPI", "Is", "Ii");

    checkBisect(lastPointName, "BisArGoMe", "Ar", "ArPMp/PMiMe", "Me");
    checkMiddlePoint(lastPointName, "MidPogMe", "Pog", "Me");
    checkAddPerpendicular(lastPointName, "PenPogMe", "Pog", "Me");


    checkPointsConnection(lastPointName, "A", "Pog");
    checkPointsConnection(lastPointName, "Is", "pIs/APog");
    checkPointsConnection(lastPointName, "Ii", "pIi/APog");
    checkPointsConnection(lastPointName, "Is", "ApIs");
    checkPointsConnection(lastPointName, "Ii", "ApIi");
    checkPointsConnection(lastPointName, "Is", "ApIsIs/IiApIi");
    checkPointsConnection(lastPointName, "Ii", "ApIsIs/IiApIi");
    checkPointsConnection(lastPointName, "ApIi", "APog/IiApIi");
    checkPointsConnection(lastPointName, "Or", "Po");
    checkPointsConnection(lastPointName, "OcPM", "OcPI");
    checkPointsConnection(lastPointName, "OcPM", "OcPPr");
    checkPointsConnection(lastPointName, "StS", "pStS/OcPMOcPPr");
    checkPointsConnection(lastPointName, "Prn", "Pog'");
    checkPointsConnection(lastPointName, "Li", "pLi/PrnPog'");
    checkPointsConnection(lastPointName, "N", "Pog");
    checkPointsConnection(lastPointName, "A", "pA/NPog");
    checkPointsConnection(lastPointName, "N", "A");
    checkPointsConnection(lastPointName, "Or", "Po");
    checkPointsConnection(lastPointName, "PoOr/NA", "Po");
    checkPointsConnection(lastPointName, "Or", "Po");
    checkPointsConnection(lastPointName, "ArPMp/PMiMe", "ArPMp/PMiMeMe/PogN");
    checkPointsConnection(lastPointName, "ArPMp/PMiMeMe/PogN", "N");
    checkPointsConnection(lastPointName, "Ba", "N");
    checkPointsConnection(lastPointName, "Pt", "Gn");
    checkPointsConnection(lastPointName, "Or", "OrPo/PMiMe");
    checkPointsConnection(lastPointName, "OrPo/PMiMe", "Me");
    checkPointsConnection(lastPointName, "Ar", "ArPMp/PMiMe");
    checkPointsConnection(lastPointName, "ArPMp/PMiMe", "Me");
    checkPointsConnection(lastPointName, "ANS", "ANSPNS/GoGn");
    checkPointsConnection(lastPointName, "ANSPNS/GoGn", "Gn");
    checkPointsConnection(lastPointName, "N", "S");
    checkPointsConnection(lastPointName, "S", "Ar");
    checkPointsConnection(lastPointName, "Ar", "Go");
    checkPointsConnection(lastPointName, "ArPMp/PMiMe", "N");
    checkPointsConnection(lastPointName, "Go", "Gn");
    checkPointsConnection(lastPointName, "N", "Me");
    checkPointsConnection(lastPointName, "ANS", "Me");
    checkPointsConnection(lastPointName, "ArPMp/PMiMe", "BisArGoMe");
    checkPointsConnection(lastPointName, "PenPogMe", "MidPogMe");
}

void CalculationJRA::checkAngle(const QString &checkPoint, const QString &dataName, const QString &pName1, const QString &pName2, const QString &pName3)
{
    CalculationBase::checkAngle(checkPoint, dataName, pName1, pName2, pName3);
    if (checkPoint == pName1 || checkPoint == pName2 || checkPoint == pName3)
    {
        checkSum(dataName, "Jarabak", QVector<QString>({"SN-Ar(deg)", "ArticularAngle", "Ar-Go-Me2(deg)"}));
    }
}

void CalculationJRA::checkSum(const QString &checkResult, const QString& resultName, const QVector<QString>& results)
{
    for (auto &it : results)
    {
        if (checkResult == it)
        {
            resultsTable_[resultName].value = calculateSum(results);
            return updateResult(resultName);
        }
    }
}

QString CalculationJRA::calculateSum(const QVector<QString> &results)
{
    float result = 0;
    bool ok = false;
    for (auto &it : results)
    {
        result += resultsTable_[it].value.toFloat(&ok);
        if (!ok)
        {
//            qDebug() << "CHECK" << resultsTable_[it].value;
            return "";
        }
    }
//    qDebug() << "RESULT READY";
    return QString::number(result);
}
