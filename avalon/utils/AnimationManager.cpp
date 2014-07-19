#include <avalon/utils/AnimationManager.h>

#include <avalon/utils/OnCleanupCallback.h>

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

    Vector<SpriteFrame*> frames;
    for (auto& name : frameNames) {
        auto frame = cache->getSpriteFrameByName(name.c_str());
        if (!frame) {
            continue;
        }
        frames.pushBack(frame);
    }

    auto animation = Animate::create(Animation::createWithSpriteFrames(frames, speed));
    ++actionTagId;
    animation->setTag(actionTagId);
    animation->retain();
    if (!retainedSprites.count(&target)) {
        retainedSprites.insert(&target);
        target.retain();
        addReleaseOnCleanup(target);
    }
    animations[animationId] = {animation, &target, actionTagId};
}

void AnimationManager::addReleaseOnCleanup(cocos2d::Sprite& target)
{
    auto onCleanup = OnCleanupCallback::create();
    target.addChild(onCleanup);

    onCleanup->set("", [this](const std::string& name, cocos2d::Node& node) {
        auto spriteNode = dynamic_cast<cocos2d::Sprite*>(&node);
        if (!spriteNode) {
            return;
        }

        std::vector<int> removeIds;
        for (auto& animation : animations) {
            auto id = animation.first;
            auto sequence = animation.second;

            if (sequence.target == spriteNode) {
                sequence.animation->release();
                removeIds.push_back(id);
            }
        }

        for (auto& id : removeIds) {
            animations.erase(id);
        }

        spriteNode->release();
        retainedSprites.erase(spriteNode);
    });
}

void AnimationManager::start(int animationId, bool loop)
{
    if (!animations.count(animationId)) {
        throw new std::runtime_error("No animation found");
    }

    auto sequence = &animations[animationId];
    sequence->target->stopActionByTag(sequence->actionTagId);

    lastAnimationId = animationId;
    if (loop) {
        auto action = RepeatForever::create(sequence->animation);
        action->setTag(sequence->actionTagId);
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
    sequence->target->stopActionByTag(sequence->actionTagId);
}

void AnimationManager::stopAll()
{
    for (auto& animation : animations) {
        stop(animation.first);
    }
}

bool AnimationManager::isRunning(int animationId)
{
    if (!animations.count(animationId)) {
        throw new std::runtime_error("No animation found");
    }
    
    auto sequence = &animations[animationId];
    return !!sequence->target->getActionByTag(sequence->actionTagId);
}

cocos2d::Animate& AnimationManager::getAnimation(int animationId)
{
    if (!animations.count(animationId)) {
        throw new std::runtime_error("No animation found");
    }
    
    return *animations[animationId].animation;
}

int AnimationManager::getLastAnimationId()
{
    return lastAnimationId;
}

} // namespace utils
} // namespace avalon
