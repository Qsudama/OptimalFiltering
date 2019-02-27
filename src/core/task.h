#ifndef TASK_H
#define TASK_H

#include "function_time.h"
#include "info.h"
#include "src/array.h"
#include "src/config.h"
#include "src/math/matrix.h"
#include "src/math/random.h"
#include "src/math/statistic.h"
#include <map>
#include <memory>

#include "src/helpers/log_in_file_manager.h"
#include "src/helpers/log_in_console_manager.h"

using Math::Matrix;
using Math::Vector;
using Math::RowVector;


namespace Core
{


//! \brief Тип параметров задачи.
using TaskParameters = std::map<std::string, double>;

//! \brief Тип умного указатель на TaskParameters.
using PtrTaskParameters = std::shared_ptr<TaskParameters>;


//! \brief Базовый класс для всех задач

class Task : public FunctionTime
{

public:
    //! \brief Конструктор.
    Task();

    //! \brief Деструктор.
    virtual ~Task();

    Task(const Task &) = delete;
    Task &operator=(const Task &) = delete;

    //! \brief Возвращает набор специфических параметров данной задачи.
    PtrTaskParameters params();

    //! \brief Возвращает набор специфических констант данной задачи.
    PtrTaskParameters consts() const;

    //! \brief Возвращает строковую информацию о данной задаче (имя, тип).
    PtrInfo info() const;

    /*!
     \brief Возвращает состояние объекта в начальный момент времени.

     \return \f$x_0 \sim \pi(\cdot)\f$.

     По-умолчанию \f$\pi(\cdot) = N(m_0, s_0^2)\f$ - гауссовское распределение.

     Если начальное состояние задается иначе в конкретной задаче, x0() необходимо переопределить.
    */
    virtual Math::Vector x0() const;

    //! \brief Возвращает размерность вектора состояния объекта \f$X\f$.
    long dimX() const;

    //! \brief Возвращает размерность вектора состояния измерителя \f$Y\f$.
    long dimY() const;

    //! \brief Возвращает размерность вектора шума \f$V\f$.
    long dimV() const;

    //! \brief Возвращает размерность вектора шума \f$W\f$.
    long dimW() const;

    //! \brief Возвращает начальное математическое ожидание \f$M[X_0]\f$ вектора состояния объекта \f$X_0\f$.
    const Math::Vector &meanX0() const;

    //! \brief Возвращает математическое ожидание \f$M[V]\f$ вектора шума \f$V\f$.
    const Math::Vector &meanV() const;

    //! \brief Возвращает математическое ожидание \f$M[W]\f$ вектора шума \f$W\f$.
    const Math::Vector &meanW() const;

    //! \brief Возвращает начальную дисперсию \f$D[X_0]\f$ вектора состояния объекта \f$X_0\f$.
    const Math::Matrix &varX0() const;

    //! \brief Возвращает дисперсию \f$D[V]\f$ вектора шума \f$V\f$.
    const Math::Matrix &varV() const;

    //! \brief Возвращает дисперсию \f$D[W]\f$ вектора шума \f$W\f$.
    const Math::Matrix &varW() const;

    //! \brief Устанавливает начальное математическое ожидание \f$M[X_0]\f$ вектора состояния объекта \f$X_0\f$.
    void setMeanX0(const Math::Vector &m);

    //! \brief Устанавливает математическое ожидание \f$M[V]\f$ вектора шума \f$V\f$.
    void setMeanV(const Math::Vector &m);

    //! \brief Устанавливает математическое ожидание \f$M[W]\f$ вектора шума \f$W\f$.
    void setMeanW(const Math::Vector &m);

    //! \brief Устанавливает начальную дисперсию \f$D[X_0]\f$ вектора состояния объекта \f$X_0\f$.
    void setVarX0(const Math::Matrix &D);

    //! \brief Устанавливает дисперсию \f$D[V]\f$ вектора шума \f$V\f$.
    void setVarV(const Math::Matrix &D);

    //! \brief Устанавливает дисперсию \f$D[W]\f$ вектора шума \f$W\f$.
    void setVarW(const Math::Matrix &D);

    /*!
     \brief Изменяет значение параметра.

     \param key - ключ, по которому производится поиск параметра из набора.
     \param value - значение, которое будет присвоено элементу с ключом key.

     \warning Если параметр с данным ключом не найден, пользователь не будет проинформирован об этом.
    */
    void changeParameter(const std::string &key, double value);

    //! \brief Менеджер для логирования в консоль.
    LogInConsoleManager& logInstance;

protected:
    //! \brief Загружает данные в локальные переменные после изменения параметров.
    virtual void loadParams();


protected:
    PtrTaskParameters m_params; /*!< Спцифические параметры задачи. */
    PtrTaskParameters m_consts; /*!< Спцифические константы задачи. */
    PtrInfo           m_info;   /*!< Информация о задаче. */


    long m_dimX; /*!< Размерность вектора \f$X\f$. */
    long m_dimY; /*!< Размерность вектора \f$Y\f$. */
    long m_dimV; /*!< Размерность вектора \f$V\f$. */
    long m_dimW; /*!< Размерность вектора \f$W\f$. */

    Math::Vector m_meanX0; /*!< Математическое ожидание \f$M[X_0]\f$. */
    Math::Vector m_meanV;  /*!< Математическое ожидание \f$M[V]\f$. */
    Math::Vector m_meanW;  /*!< Математическое ожидание \f$M[W]\f$. */

    Math::Matrix m_varX0; /*!< Дисперсия \f$D[X_0]\f$. */
    Math::Matrix m_varV;  /*!< Дисперсия \f$D[V]\f$. */
    Math::Matrix m_varW;  /*!< Дисперсия \f$D[W]\f$. */

    Math::MultivariateNormalDistribution m_normalRand; /*!< Генератор гауссовских случайных векторов. */
};


/*!
 \brief Тип умного указателя на Task.
*/
using PtrTask = std::shared_ptr<Task>;


} // end Core

#endif // TASK_H
