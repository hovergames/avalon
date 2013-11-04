#include <avalon/utils/AnimationManager.h>

using namespace cocos2d;

namespace avalon {
namespace utils {

AnimationManager::~AnimationManager()
{
    for (auto& animation : animations) {
        animation.second.animation->release();
    }
}

void AnimationManager::addAnimation(int animationId, const std::string& spriteFramesFile, const std::list<std::string>& frameNames, cocos2d::Sprite& target, float speed)
{
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile(spriteFramesFile.c_str());
    
    addAnimation(animationId, frameNames, target, speed);
}

void AnimationManager::addAnimation(int animationId, const std::list<std::string>& frameNames, cocos2d::Sprite& target, float speed)
{
    if (animations.count(animationId)) {
        animations[animationId].animation->release();
        animations.erase(animationId);
    }

    auto cache = SpriteFrameCache::getInstance();
    auto frames = Array::createWithCapacity(frameNames.size());

    for (auto& name : frameNames) {
        auto frame = cache->getSpriteFrameByName(name.c_str());
        if (!frame) {
            continue;
        }
        frames->addObject(frame);
    }

    auto animation = Animate::create(Animation::createWithSpriteFrames(frames, speed));
    animation->setTag(actionTagId);
    animation->retain();

    animations[animationId] = {animation, &target};
}

void AnimationManager::start(int animationId, bool loop)
{
    if (!animations.count(animationId)) {
        throw new std::runtime_error("No animation found");
    }

    auto sequence = &animations[animationId];
    sequence->target->stopActionByTag(actionTagId);

    if (loop) {
        auto action = RepeatForever::create(sequence->animation);
        action->setTag(actionTagId);
        sequence->target->runAction(action);
    } else {
        sequence->target->runAction(sequence->animation);
    }
}

void AnimationManager::stop(int animationId)
{
    if (!animations.count(animationId)) {
        throw new std::runtime_error("No animation found");
    }

    auto sequence = &animations[animationId];
    sequence->target->stopActionByTag(actionTagId);
}

bool AnimationManager::isRunning(int animationId)
{
    if (!animations.count(animationId)) {
        throw new std::runtime_error("No animation found");
    }

    auto sequence = &animations[animationId];
    return !!sequence->target->getActionByTag(actionTagId);
}

cocos2d::Animate& AnimationManager::getAnimation(int animationId)
{
    if (!animations.count(animationId)) {
        throw new std::runtime_error("No animation found");
    }

    return *animations[animationId].animation;
}

} // namespace utils
} // namespace avalon
