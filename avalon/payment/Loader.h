#ifndef AVALON_PAYMENT_LOADER_H
#define AVALON_PAYMENT_LOADER_H

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <avalon/io/IniReader.h>

namespace avalon {
namespace payment {

class Manager;

/**
 * Translates the content of the given ini-file into the right setup of
 * payment::Manager with payment::Product and/or payment::ProductConsumable.
 */
class Loader : boost::noncopyable
{
public:
    static boost::shared_ptr<Manager> globalManager;

    explicit Loader(const char* iniFile);
    boost::shared_ptr<Manager> getManager() const;

private:
    boost::shared_ptr<Manager> manager;
    io::IniReader config;

    const char* detectProductId(const char* section);
};

} // namespace payment
} // namespace avalon

#endif /* AVALON_PAYMENT_LOADER_H */
