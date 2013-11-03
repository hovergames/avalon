#ifndef AVALON_UTILS_ANIMATIONMANAGER_H
#define AVALON_UTILS_ANIMATIONMANAGER_H

#include <map>
#include "cocos2d.h"

namespace avalon {
namespace utils {

class AnimationManager
{
private:
    struct AnimationSequence
    {
        cocos2d::Animate* animation;
        cocos2d::Sprite* target;
        cocos2d::Action* action;
    };

    AnimationSequence* currentAnimation = nullptr;
    std::map<int, AnimationSequence> animations;

public:
    int actionTagId = -2;

    ~AnimationManager();

    void addAnimation(int animationId, std::list<std::string> frameNames, cocos2d::Sprite *target, float speed = 1.0);
    void start(int animationId, bool loop = false);
    bool isRunning();
    void stop();
    cocos2d::Animate* getCurrentAnimation();
    cocos2d::Animate* getAnimation(int animationId);
};

} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_ANIMATIONMANAGER_H */
