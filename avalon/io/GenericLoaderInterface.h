#ifndef AVALON_IO_GENERICLOADERINTERFACE_H
#define AVALON_IO_GENERICLOADERINTERFACE_H

namespace avalon {
namespace io {

class GenericLoaderInterface
{
public:
    virtual void dispatchPendingProperties() = 0;
};

} // namespace io
} // namespace avalon

#endif /* AVALON_IO_GENERICLOADERINTERFACE_H */