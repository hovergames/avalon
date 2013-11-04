#include <avalon/physics/ContactContainer.h>

#include <avalon/physics/Box2dContainer.h>

namespace avalon {
namespace physics {

ContactContainer::ContactContainer(Box2dContainer* container, b2Contact* contact, bool iAmInFixtureA)
: container(container)
, contact(contact)
, iAmInFixtureA(iAmInFixtureA)
{
}

Box2dContainer& ContactContainer::getContainer()
{
    return *container;
}

const Box2dContainer& ContactContainer::getContainer() const
{
    return *container;
}

b2Contact& ContactContainer::getContact()
{
    return *contact;
}

const b2Contact& ContactContainer::getContact() const
{
    return *contact;
}

b2Fixture& ContactContainer::getMyFixture()
{
    if (iAmInFixtureA) {
        return *contact->GetFixtureA();
    } else {
        return *contact->GetFixtureB();
    }
}

const b2Fixture& ContactContainer::getMyFixture() const
{
    if (iAmInFixtureA) {
        return *contact->GetFixtureA();
    } else {
        return *contact->GetFixtureB();
    }
}

b2Fixture& ContactContainer::getOpponentFixture()
{
    if (iAmInFixtureA) {
        return *contact->GetFixtureB();
    } else {
        return *contact->GetFixtureA();
    }
}

const b2Fixture& ContactContainer::getOpponentFixture() const
{
    if (iAmInFixtureA) {
        return *contact->GetFixtureB();
    } else {
        return *contact->GetFixtureA();
    }
}

const int32 ContactContainer::getMyChildIndex() const
{
    if (iAmInFixtureA) {
        return contact->GetChildIndexA();
    } else {
        return contact->GetChildIndexB();
    }
}

const int32 ContactContainer::getOpponentChildIndex() const
{
    if (iAmInFixtureA) {
        return contact->GetChildIndexB();
    } else {
        return contact->GetChildIndexA();
    }
}

} // namespace physics
} // namespace avalon
