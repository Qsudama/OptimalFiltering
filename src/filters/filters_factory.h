#ifndef FILTERS_H
#define FILTERS_H

#include "filters_identifiers.h"
#include "src/core/types_info.h"
#include "src/filters/continuous/c_aof.h"
#include "src/filters/continuous/c_fos.h"
#include "src/filters/continuous_discrete/cd_aof.h"
#include "src/filters/continuous_discrete/cd_dfos.h"
#include "src/filters/continuous_discrete/cd_dfosbo.h"
#include "src/filters/continuous_discrete/cd_fos.h"
#include "src/filters/discrete/d_aof.h"
#include "src/filters/discrete/d_fos.h"
#include "src/filters/discrete/d_mfos.h"
#include "src/filters/discrete/d_duof.h"
#include "src/filters/discrete/d_dfmp.h"
#include "src/filters/discrete/d_fkp.h"
#include "src/filters/discrete/d_dfkp.h"
#include "src/filters/discrete/d_mdfmp.h"
#include "src/filters/logic_dynamic/ld_aof.h"
#include "src/filters/logic_dynamic/ld_fos.h"
#include "src/filters/logic_dynamic/ld_fkp_fbp.h"

//! \brief Модуль, содержащий реализации конкретных алгоритмов фильтрации.

namespace Filters
{

//! \brief Фабричный класс для создания экземпляров классов фильтров.

class FilterFactory
{
public:
    /*!
     \brief Создает экземпляр фильтра по входным параметрам.

     \param type - тип фильтра (непрерывный, дискретный, ...).
     \param id - идентификатор фильтра (АОФ, ФМП и т.д.).
     \param params - указатель на параметры, которые будет использовать фильтр.
     \param task - указатель на экземпляр решаемой задачи
     \return указатель на созданный экземпляр фильтра.
    */
    static Core::PtrFilter create(Core::FILTER_TYPE type, FILTER_ID id, Core::PtrFilterParameters params,
                                  Core::PtrTask task);


private:
    //! \brief Вспомогательный метод для создания непрерывных фильтров оптимальной структуры.
    static Core::PtrFilter createContinuous(FILTER_ID id, Core::PtrFilterParameters params, Core::PtrTask task);

    //! \brief Вспомогательный метод для создания непрерывно-дискретных фильтров оптимальной структуры.
    static Core::PtrFilter createContinuousDiscrete(FILTER_ID id, Core::PtrFilterParameters params, Core::PtrTask task);

    //! \brief Вспомогательный метод для создания дискретных фильтров оптимальной структуры.
    static Core::PtrFilter createDiscrete(FILTER_ID id, Core::PtrFilterParameters params, Core::PtrTask task);

    //! \brief Вспомогательный метод для создания логико-динамических фильтров оптимальной структуры.
    static Core::PtrFilter createLogicDynamic(FILTER_ID id, Core::PtrFilterParameters params, Core::PtrTask task);
};


} // end Filters


#endif // FILTERS_H
