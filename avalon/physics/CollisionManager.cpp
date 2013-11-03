#include <avalon/physics/CollisionManager.h>

namespace avalon {
namespace physics {

CollisionManager::CollisionManager(Box2dContainer& box2dContainer)
: box2dContainer(box2dContainer)
{
}

void CollisionManager::BeginContact(b2Contact* contact)
{
    auto handled = false;
    for (auto& callback : listBeginContact) {
        handled = callback(*contact) || handled;
    }

    if (!handled && fallback) {
        fallback->onBeginContact(box2dContainer, *contact);
    }
}

void CollisionManager::EndContact(b2Contact* contact)
{
    auto handled = false;
    for (auto& callback : listEndContact) {
        handled = callback(*contact) || handled;
    }

    if (!handled && fallback) {
        fallback->onEndContact(box2dContainer, *contact);
    }
}

void CollisionManager::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    auto handled = false;
    for (auto& callback : listPreSolve) {
        handled = callback(*contact, *oldManifold) || handled;
    }

    if (!handled && fallback) {
        fallback->onPreSolve(box2dContainer, *contact, *oldManifold);
    }
}

void CollisionManager::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    auto handled = false;
    for (auto& callback : listPostSolve) {
        handled = callback(*contact, *impulse) || handled;
    }

    if (!handled && fallback) {
        fallback->onPostSolve(box2dContainer, *contact, *impulse);
    }
}

} // namespace physics
} // namespace avalon