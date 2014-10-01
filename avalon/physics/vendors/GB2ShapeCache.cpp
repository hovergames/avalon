//
//  GB2ShapeCache-x.cpp
//
//  Loads physics sprites created with http://www.PhysicsEditor.de
//  To be used with cocos2d-x
//
//  Generic Shape Cache for box2d
//
//  Created by Thomas Broquist
//
//      http://www.PhysicsEditor.de
//      http://texturepacker.com
//      http://www.code-and-web.de
//
//    Copyright (c) 2012 Chris Hannon / channon.us
//    Copyright (c) 2013 Tomoaki Shimizu / tks2.net
//
//  All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include "GB2ShapeCache.h"
#include "Box2D/Box2D.h"

using namespace cocos2d;
using namespace gbox2d;

/**
 * Internal class to hold the fixtures
 */
class FixtureDef {
public:
    FixtureDef()
    : next(nullptr) {}

    ~FixtureDef() {
        delete next;
        delete fixture.shape;
    }

    FixtureDef *next;
    b2FixtureDef fixture;
    int callbackData;
};

class BodyDef {
public:
    BodyDef()
    : fixtures(nullptr) {}

    ~BodyDef() {
        if (fixtures)
            delete fixtures;
    }

    FixtureDef *fixtures;
    Point anchorPoint;
};

static GB2ShapeCache *_sharedGB2ShapeCache = nullptr;

GB2ShapeCache* GB2ShapeCache::getInstance(void) {
    if (!_sharedGB2ShapeCache) {
        _sharedGB2ShapeCache = new GB2ShapeCache();
        _sharedGB2ShapeCache->init();
    }

    return _sharedGB2ShapeCache;
}

bool GB2ShapeCache::init() {
    return true;
}

void GB2ShapeCache::reset() {
    std::map<std::string, BodyDef *>::iterator iter;
    for (iter = shapeObjects.begin() ; iter != shapeObjects.end() ; ++iter) {
        delete iter->second;
    }
    shapeObjects.clear();
}

void GB2ShapeCache::addFixturesToBody(b2Body *body, const std::string &shape) {
    std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find(shape);
    assert(pos != shapeObjects.end());

    auto so = (*pos).second;

    auto fix = so->fixtures;
    while (fix) {
        body->CreateFixture(&fix->fixture);
        fix = fix->next;
    }
}

cocos2d::Point GB2ShapeCache::anchorPointForShape(const std::string &shape) {
    auto pos = shapeObjects.find(shape);
    assert(pos != shapeObjects.end());

    auto bd = (*pos).second;
    return bd->anchorPoint;
}

void GB2ShapeCache::addShapesWithFile(const std::string &plist) {

    auto dict = Dictionary::createWithContentsOfFileThreadSafe(plist.c_str());
    CCAssert(dict != nullptr, "Shape-file not found"); // not triggered - cocos2dx delivers empty dict if non was found
    CCAssert(dict->count() != 0, "plist file empty or not existing");

    auto metadataDict = (Dictionary*)dict->objectForKey("metadata");
    auto format = static_cast<String*>(metadataDict->objectForKey("format"))->intValue();
    ptmRatio = static_cast<String*>(metadataDict->objectForKey("ptm_ratio"))->floatValue();
    CCAssert(format == 1, "Format not supported");

    auto bodyDict = (Dictionary*)dict->objectForKey("bodies");

    b2Vec2 vertices[b2_maxPolygonVertices];

    DictElement* pElement = nullptr;
    CCDICT_FOREACH(bodyDict, pElement)
    {
        auto bodyDef = new BodyDef();

        auto bodyName = ccs(pElement->getStrKey());

        auto bodyData = (Dictionary*)pElement->getObject();
        bodyDef->anchorPoint = PointFromString(static_cast<String*>(bodyData->objectForKey("anchorpoint"))->getCString());

        auto fixtureList = (Array*)bodyData->objectForKey("fixtures");
        auto nextFixtureDef = &(bodyDef->fixtures);

        Object* fixture = nullptr;
        CCARRAY_FOREACH(fixtureList, fixture)
        {

            b2FixtureDef basicData;
            auto fixtureData = (Dictionary*)fixture;
            int callbackData = 0;

            basicData.filter.categoryBits = static_cast<String *>(fixtureData->objectForKey("filter_categoryBits"))->intValue();
            basicData.filter.maskBits = static_cast<String *>(fixtureData->objectForKey("filter_maskBits"))->intValue();
            basicData.filter.groupIndex = static_cast<String *>(fixtureData->objectForKey("filter_groupIndex"))->intValue();
            basicData.friction = static_cast<String *>(fixtureData->objectForKey("friction"))->floatValue();
            basicData.density = static_cast<String *>(fixtureData->objectForKey("density"))->floatValue();
            basicData.restitution = static_cast<String *>(fixtureData->objectForKey("restitution"))->floatValue();
            basicData.isSensor = (bool)static_cast<String *>(fixtureData->objectForKey("isSensor"))->intValue();

            // hotfix for ios
            if (static_cast<String *>(fixtureData->objectForKey("isSensor"))->compare("true") == 0) {
                basicData.isSensor = true;
            }

            std::string fixtureType = static_cast<String *>(fixtureData->objectForKey("fixture_type"))->getCString();

            if (fixtureType == "POLYGON") {
                //CCDictionary *polygons = (CCDictionary *)fixtureData->objectForKey("polygons");
                auto polygons = (Array *)fixtureData->objectForKey("polygons");
                //CCDictElement *polygon = nullptr;
                Object *polygon = nullptr;
                //CCDICT_FOREACH(polygons, polygon)
                CCARRAY_FOREACH(polygons, polygon)
                {
                    auto fix = new FixtureDef();
                    fix->fixture = basicData; // copy basic data
                    fix->callbackData = callbackData;

                    b2PolygonShape *polyshape = new b2PolygonShape();
                    int vindex = 0;

                    //CCDictionary *polygonData = (CCDictionary *)polygon->getObject();
                    auto polygonData = (Array *)polygon;

                    assert(polygonData->count() <= b2_maxPolygonVertices);

                    //CCDictElement *offset = nullptr;
                    Object *offset = nullptr;
                    //CCDICT_FOREACH(polygonData, offset)
                    CCARRAY_FOREACH(polygonData, offset)
                    {

                        auto pStr = (String *)offset;
                        auto p = PointFromString(pStr->getCString());

                        vertices[vindex].x = (p.x / ptmRatio) ;
                        vertices[vindex].y = (p.y / ptmRatio) ;
                        vindex++;

                    }

                    polyshape->Set(vertices, vindex);
                    fix->fixture.shape = polyshape;

                    *nextFixtureDef = fix;
                    nextFixtureDef = &(fix->next);
                }

            } else if (fixtureType == "CIRCLE") {
                auto fix = new FixtureDef();
                fix->fixture = basicData; // copy basic data
                fix->callbackData = callbackData;

                auto circleData = (Dictionary *)fixtureData->objectForKey("circle");

                auto circleShape = new b2CircleShape();

                circleShape->m_radius = static_cast<String *>(circleData->objectForKey("radius"))->floatValue() / ptmRatio;
                auto p = PointFromString(static_cast<String *>(circleData->objectForKey("position"))->getCString());
                circleShape->m_p = b2Vec2(p.x / ptmRatio, p.y / ptmRatio);
                fix->fixture.shape = circleShape;

                // create a list
                *nextFixtureDef = fix;
                nextFixtureDef = &(fix->next);

            } else {
                CCAssert(0, "Unknown fixtureType");
            }

            // add the body element to the hash
            shapeObjects[bodyName->getCString()] = bodyDef;

        }

    }

}