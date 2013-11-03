#include <avalon/physics/CollisionManager.h>

namespace avalon {
namespace physics {

CollisionManager::CollisionManager(Box2dContainer& box2dContainer)
: box2dContainer(box2dContainer)
{
}

void CollisionManager::BeginContact(b2Contact* contact)
{
    for (auto& callback : listBeginContact) {
        callback(*contact);
    }
}

void CollisionManager::EndContact(b2Contact* contact)
{
    for (auto& callback : listEndContact) {
        callback(*contact);
    }
}

void CollisionManager::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    for (auto& callback : listPreSolve) {
        callback(*contact, *oldManifold);
    }
}

void CollisionManager::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    for (auto& callback : listPostSolve) {
        callback(*contact, *impulse);
    }
}

} // namespace physics
} // namespace avalon