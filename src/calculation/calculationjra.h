#ifndef CALCULATIONFACE_H
#define CALCULATIONFACE_H

#include "calculationbase.h"

class CalculationJRA: public CalculationBase
{
    public:
        explicit CalculationJRA(IFrontendConnector& frontendConnector);
        ~CalculationJRA() override;

        void updateCalculation(const QString& lastPointName) override;

    private:
        void checkAngle(const QString& checkPoint, const QString& dataName, const QString& pName1, const QString& pName2, const QString& pName3) override;
        void checkSum(const QString& checkResult, const QString& resultName, const QVector<QString>& results);

        //FIXME: better solution
        QString calculateSum(const QVector<QString>& results);

};

#endif // CALCULATIONFACE_H
