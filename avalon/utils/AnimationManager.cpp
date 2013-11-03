#include <avalon/utils/AnimationManager.h>

namespace avalon {
namespace utils {

AnimationManager::~AnimationManager()
{
    for (auto& animation : animations) {
        CC_SAFE_RELEASE(animation.second.animation);
    }

}

void AnimationManager::addAnimation(int animationId, std::list<std::string> frameNames, cocos2d::Sprite *target, float speed)
{
    if (animations.count(animationId)) {
        CC_SAFE_RELEASE(animations.at(animationId).animation);
        animations.erase(animationId);
    }

    cocos2d::Array* animFrames = cocos2d::Array::createWithCapacity(frameNames.size());

    for (auto& frameName : frameNames) {
        cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName.c_str());
        animFrames->addObject(frame);
    }

    cocos2d::Animate* animation = cocos2d::Animate::create(cocos2d::Animation::createWithSpriteFrames(animFrames, speed));
    animation->retain();

    animations.insert(std::make_pair(animationId, AnimationSequence{animation, target, nullptr}));
}

void AnimationManager::start(int animationId, bool loop)
{
    AnimationSequence* animationSequence = &animations.at(animationId);

    if (currentAnimation == animationSequence && !currentAnimation->animation->isDone())
        return;

    animationSequence->target->stopActionByTag(actionTagId);

    if (loop) {
        animationSequence->action = cocos2d::RepeatForever::create(animationSequence->animation);
        animationSequence->action->setTag(actionTagId);
        animationSequence->target->runAction(animationSequence->action);
    } else {
        animationSequence->target->runAction(animationSequence->animation);
    }

    currentAnimation = animationSequence;
}

cocos2d::Animate* AnimationManager::getCurrentAnimation()
{
    return currentAnimation->animation;
}

cocos2d::Animate* AnimationManager::getAnimation(int animationId)
{
    return animations.at(animationId).animation;
}


} // namespace utils
} // namespace avalon
