/****************************************************************************
 * B2DebugDrawLayer.h
 *
 * Created by Stefan Nguyen on Oct 8, 2012.
 *
 * Copyright Vinova Pte. Ltd. All rights reserved.
 ****************************************************************************/

#ifndef __B2_DEBUG_DRAW_LAYER_H__
#define __B2_DEBUG_DRAW_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Box2d/Box2d.h"
#include "GLES-Render.h"

class B2DebugDrawLayer : public cocos2d::Layer
{
    b2World* mB2World;
    GLESDebugDraw* mB2DebugDraw;
    const float mPtmRatio;
 
public:
    B2DebugDrawLayer(b2World* pB2World, float pPtmRatio);
    static B2DebugDrawLayer* create(b2World* pB2World, float pPtmRatio);
    bool init() override;
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
};

#endif // __B2_DEBUG_DRAW_LAYER_H__
