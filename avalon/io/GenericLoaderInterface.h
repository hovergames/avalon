#ifndef AVALON_IO_GENERICLOADERINTERFACE_H
#define AVALON_IO_GENERICLOADERINTERFACE_H

#include <boost/any.hpp>

namespace avalon {

namespace physics {
    class Box2dContainer;
}

namespace io {

class GenericLoaderInterface
{
public:
    typedef std::map<std::string, boost::any> Dictionary;
    struct Configuration
    {
        const Dictionary& settings;
        avalon::physics::Box2dContainer* box2dContainer;
    };

    virtual void dispatchPendingProperties(avalon::physics::Box2dContainer* box2dContainer) = 0;
};

} // namespace io
} // namespace avalon

#endif /* AVALON_IO_GENERICLOADERINTERFACE_H */