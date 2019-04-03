#ifndef INFO_H
#define INFO_H

#include <memory>
#include <string>


namespace Core
{


//! \brief Класс для хранения строковой информации о чем-либо.

class Info
{

public:
    //! \brief Конструктор по-умолчанию.
    Info();

    //! \brief Конструктор.
    Info(const std::string &name, const std::string &dimension, const std::string &type);

    //! \brief Возвращает строку с именем + размерность.
    std::string full_name();

    //! \brief Возвращает строку с именем.
    const std::string &name() const;

    //! \brief Возвращает строку с размерностью.
    const std::string &dimension() const;

    //! \brief Возвращает строку с типом.
    const std::string &type() const;

    //! \brief Устонавливает новое имя.
    void setName(const std::string &name);

    //! \brief Устонавливает новую размерность.
    void setDimension(const std::string &name);

    //! \brief Устонавливает новый тип.
    void setType(const std::string &type);


private:
    std::string m_name; /*!< Имя. */
    std::string m_dimension; /*!< Размерность. */
    std::string m_type; /*!< Тип. */
};


//! \brief Тип умного указателя на Info.
using PtrInfo = std::shared_ptr<Info>;


} // end Core


#endif // INFO_H
