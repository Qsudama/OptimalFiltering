#include "info.h"


namespace Core
{


Info::Info()
    : m_name("")
    , m_dimension("")
    , m_type("")
{
}

Info::Info(const std::string &name, const std::string &dimension, const std::string &type)
    : m_name(name)
    , m_dimension(dimension)
    , m_type(type)
{
}

std::string Info::full_name()
{
    std::string full_name = m_name + " " + m_dimension;
    return full_name;
}

const std::string &Info::name() const
{
    return m_name;
}

const std::string &Info::dimension() const
{
    return m_dimension;
}

const std::string &Info::type() const
{
    return m_type;
}

void Info::setName(const std::string &name)
{
    m_name = name;
}

void Info::setDimension(const std::string &name)
{
    m_dimension = name;
}

void Info::setType(const std::string &type)
{
    m_type = type;
}


} // end Core
