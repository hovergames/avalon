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
        int actionTagId;
    };

    std::map<int, AnimationSequence> animations;
    std::set<cocos2d::Sprite*> retainedSprites;

    int actionTagId = 0;

    int lastAnimationId = -1;

    void addReleaseOnCleanup(cocos2d::Sprite& target);

public:
    AnimationManager(int tagId = 0) : actionTagId(tagId) {}
    ~AnimationManager();

    void addAnimation(int animationId, const std::list<std::string>& frameNames, cocos2d::Sprite& target, float speed = 1.0);
    void addAnimation(int animationId, const std::string& spriteFramesFile, const std::list<std::string>& frameNames, cocos2d::Sprite& target, float speed = 1.0);

    void start(int animationId, bool loop = false);
    void stop(int animationId);
    void stopAll();
    bool isRunning(int animationId);
    int getLastAnimationId();
    
    cocos2d::Animate& getAnimation(int animationId);
};


} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_ANIMATIONMANAGER_H */
