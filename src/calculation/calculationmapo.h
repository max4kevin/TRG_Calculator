#ifndef CALCULATIONMAPO_H
#define CALCULATIONMAPO_H

#include "calculation/calculationbase.h"

class CalculationMAPO: public CalculationBase
{
    public:
        explicit CalculationMAPO(IFrontendConnector& frontendConnector);
        ~CalculationMAPO() override;

        void updateCalculation(const QString& lastPointName) override;

    private:

        void checkAngle(const QString& checkPoint, const QString& dataName, const QString& pName1, const QString& pName2, const QString& pName3) override;
        void checkDistance(const QString& checkPoint, const QString& dataName, const QString& pName1, const QString& pName2) override;

        void checkBisect(const QString& checkPoint, const QString& pointName, const QString& pName1, const QString& pName2, const QString& pName3);
        void checkMiddlePoint(const QString& checkPoint, const QString& pointName, const QString& pName1, const QString& pName2);
        void checkAddPerpendicular(const QString& checkPoint, const QString& pointName, const QString& pName1, const QString& pName2);
};

#endif // CALCULATIONMAPO_H
