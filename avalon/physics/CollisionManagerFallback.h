#ifndef AVALON_PHYSICS_COLLISIONMANAGERFALLBACK_H
#define AVALON_PHYSICS_COLLISIONMANAGERFALLBACK_H

#include "Box2D/Box2D.h"
#include <avalon/physics/ContactContainer.h>

namespace avalon {
namespace physics {

class CollisionManagerFallback
{
public:
    virtual void onBeginContact(avalon::physics::ContactContainer& contact) = 0;
    virtual void onEndContact(avalon::physics::ContactContainer& contact) = 0;
    virtual void onPreSolve(avalon::physics::ContactContainer& contact, const b2Manifold& oldManifold) = 0;
    virtual void onPostSolve(avalon::physics::ContactContainer& contact, const b2ContactImpulse& impulse) = 0;
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_COLLISIONMANAGERFALLBACK_H */
