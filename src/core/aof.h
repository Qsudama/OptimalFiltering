#ifndef AOF_H
#define AOF_H
#include "src/core/logic_dynamic_filter.h"


class AOF:public Core::LogicDynamicFilter
{
public:

    void algorithm();

    void computeBlock1(long s);

    void computeBlock2(long s);

    void computeBlock4(long s);

    void computeBlock5(long s);

    void computeBlock6();
};

#endif // AOF_H
