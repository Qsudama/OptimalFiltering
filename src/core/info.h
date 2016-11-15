#ifndef INFO_H
#define INFO_H

#include <memory>
#include <string>


namespace Core
{


class Info
{

public:
    Info();
    Info(const std::string &name, const std::string &type);

    const std::string &name() const;
    const std::string &type() const;

    void setName(const std::string &name);
    void setType(const std::string &type);


private:
    std::string m_name;
    std::string m_type;
};


using PtrInfo = std::shared_ptr<Info>;


} // end Core


#endif // INFO_H
