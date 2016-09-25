#include "info.h"


namespace Core
{


Info::Info()
    : m_name("")
    , m_type("")
{
}

Info::Info(const std::string &name, const std::string &type)
    : m_name(name)
    , m_type(type)
{
}

const std::string &Info::name() const
{
    return m_name;
}

const std::string &Info::type() const
{
    return m_type;
}

void Info::setName(const std::string &name)
{
    m_name = name;
}

void Info::setType(const std::string &type)
{
    m_type = type;
}


} // end Core
