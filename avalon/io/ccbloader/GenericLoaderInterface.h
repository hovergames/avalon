#ifndef AVALON_IO_CCBLOADER_GENERICLOADERINTERFACE_H
#define AVALON_IO_CCBLOADER_GENERICLOADERINTERFACE_H

namespace avalon { namespace physics { class Box2dContainer; } }

namespace avalon {
namespace io {
namespace ccbloader {

class GenericLoaderInterface
{
public:
    virtual void dispatchPendingProperties(avalon::physics::Box2dContainer* box2dContainer) = 0;
};

} // namespace ccbloader
} // namespace io
} // namespace avalon

#endif /* AVALON_IO_CCBLOADER_GENERICLOADERINTERFACE_H */