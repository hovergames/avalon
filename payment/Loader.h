#ifndef AVALON_PAYMENT_LOADER_H
#define AVALON_PAYMENT_LOADER_H

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <avalon/io/IniReader.h>

using boost::shared_ptr;

namespace avalon {
namespace payment {

class Manager;

class Loader : boost::noncopyable
{
public:
    static shared_ptr<Manager> globalManager;

    Loader(const char* iniFile);
    shared_ptr<Manager> getManager() const;

private:
    shared_ptr<Manager> manager;
    IniReader config;

    const char* detectProductId(const char* section);
};

} // namespace payment
} // namespace avalon

#endif /* AVALON_PAYMENT_LOADER_H */
