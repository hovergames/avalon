#ifndef AVALON_PHYSICS_COLLISIONMANAGER_H
#define AVALON_PHYSICS_COLLISIONMANAGER_H

#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include <avalon/physics/Box2dContainer.h>
#include <avalon/physics/ContactContainer.h>
#include <boost/noncopyable.hpp>

namespace avalon { namespace physics { class CollisionManagerFallback; } }

namespace avalon {
namespace physics {

class CollisionManager : public b2ContactListener, public boost::noncopyable
{
private:
    using ContactType = std::function<bool(ContactContainer&)>;
    using PreSolveType = std::function<bool(ContactContainer&, const b2Manifold&)>;
    using PostSolveType = std::function<bool(ContactContainer&, const b2ContactImpulse&)>;

    Box2dContainer& box2dContainer;
    std::list<ContactType> listBeginContact;
    std::list<ContactType> listEndContact;
    std::list<PreSolveType> listPreSolve;
    std::list<PostSolveType> listPostSolve;

    ContactContainer contactContainer{nullptr, nullptr, true};

    template<typename A, typename B>
    bool lookup(ContactContainer& contact, A** a, B** b)
    {
        auto& fixtureA = *contact.getContact().GetFixtureA();
        auto& fixtureB = *contact.getContact().GetFixtureB();

        *a = box2dContainer.getNode<A>(fixtureA, true);
        contactContainer.iAmInFixtureA = true;
        if (!*a) {
            *a = box2dContainer.getNode<A>(fixtureB, true);
            contactContainer.iAmInFixtureA = false;
        }
        if (!*a) {
            return false;
        }

        if (contactContainer.iAmInFixtureA) {
            *b = box2dContainer.getNode<B>(fixtureB, true);
        } else {
            *b = box2dContainer.getNode<B>(fixtureA, true);
        }
        if (!*b) {
            return false;
        }

        return true;
    }

    template<typename A>
    bool lookup(ContactContainer& contact, A** a)
    {
        auto& fixtureA = *contact.getContact().GetFixtureA();
        auto& fixtureB = *contact.getContact().GetFixtureB();

        *a = box2dContainer.getNode<A>(fixtureA, true);
        contactContainer.iAmInFixtureA = true;
        if (!*a) {
            *a = box2dContainer.getNode<A>(fixtureB, true);
            contactContainer.iAmInFixtureA = false;
        }
        if (!*a) {
            return false;
        }

        return true;
    }

public:
    CollisionManagerFallback* fallback = nullptr;

    CollisionManager(Box2dContainer& box2dContainer);

    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

    template<typename A>
    void registerClass()
    {
        listBeginContact.push_back([this](ContactContainer& contact) {
            A* a = nullptr;
            if (lookup(contact, &a)) {
                a->onBeginContact(contact);
                return true;
            }
            return false;
        });

        listEndContact.push_back([this](ContactContainer& contact) {
            A* a = nullptr;
            if (lookup(contact, &a)) {
                a->onEndContact(contact);
                return true;
            }
            return false;
        });

        listPreSolve.push_back([this](ContactContainer& contact, const b2Manifold& oldManifold) {
            A* a = nullptr;
            if (lookup(contact, &a)) {
                a->onPreSolve(contact, oldManifold);
                return true;
            }
            return false;
        });

        listPostSolve.push_back([this](ContactContainer& contact, const b2ContactImpulse& impulse) {
            A* a = nullptr;
            if (lookup(contact, &a)) {
                a->onPostSolve(contact, impulse);
                return true;
            }
            return false;
        });
    }

    template<typename A, typename B>
    void registerPair()
    {
        listBeginContact.push_back([this](ContactContainer& contact) {
            A* a = nullptr; B* b = nullptr;
            if (lookup(contact, &a, &b)) {
                a->onBeginContact(*b, contact);
                return true;
            }
            return false;
        });
        
        listEndContact.push_back([this](ContactContainer& contact) {
            A* a = nullptr; B* b = nullptr;
            if (lookup(contact, &a, &b)) {
                a->onEndContact(*b, contact);
                return true;
            }
            return false;
        });
        
        listPreSolve.push_back([this](ContactContainer& contact, const b2Manifold& oldManifold) {
            A* a = nullptr; B* b = nullptr;
            if (lookup(contact, &a, &b)) {
                a->onPreSolve(*b, contact, oldManifold);
                return true;
            }
            return false;
        });
        
        listPostSolve.push_back([this](ContactContainer& contact, const b2ContactImpulse& impulse) {
            A* a = nullptr; B* b = nullptr;
            if (lookup(contact, &a, &b)) {
                a->onPostSolve(*b, contact, impulse);
                return true;
            }
            return false;
        });
    }
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_COLLISIONMANAGER_H */
