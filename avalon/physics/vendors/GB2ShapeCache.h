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

#ifndef GB2ShapeCache_x_h
#define GB2ShapeCache_x_h

#include "cocos2d.h"

class BodyDef;
class b2Body;

namespace cocos2d {
	class GB2ShapeCache {
	public:
		// Static interface
		static GB2ShapeCache* sharedGB2ShapeCache(void);

	public:
		bool init();

		void addShapesWithFile(const std::string &plist);
		void addFixturesToBody(b2Body *body, const std::string &shape);
		cocos2d::CCPoint anchorPointForShape(const std::string &shape);
		void reset();
		float getPtmRatio() { return ptmRatio; }
		~GB2ShapeCache() {}

	private:
		std::map<std::string, BodyDef *> shapeObjects;
		GB2ShapeCache(void) {}
		float ptmRatio;
	};
}

#endif