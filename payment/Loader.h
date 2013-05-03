#ifndef __Avalon__Payment__Loader__
#define __Avalon__Payment__Loader__

#include <boost/shared_ptr.hpp>
#include "Manager.h"
#include "IniReader.h"

using boost::shared_ptr;

namespace Avalon {
namespace Payment {

class Loader
{
public:
    static shared_ptr<Manager> globalManager;

    Loader(const char* iniFile);
    shared_ptr<Manager> getManager();

private:
    shared_ptr<Manager> manager;
    IniReader config;

    const char* detectProductId(const char* section);
};

} // namespace Payment
} // namespace Avalon

#endif /* __Avalon__Payment__Loader__ */