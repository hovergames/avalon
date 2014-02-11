#include "ClippingNodeRect.h"

namespace avalon {
namespace graphics {

void ClippingNodeRect::visit()
{
    kmGLPushMatrix();
    glEnable(GL_SCISSOR_TEST);


    auto glView = cocos2d::Director::getInstance()->getOpenGLView();
    glView->setScissorInPoints(
        getPositionX(), getPositionY(),
        getContentSize().width, getContentSize().height
    );
    Node::visit();

    glDisable(GL_SCISSOR_TEST);
    kmGLPopMatrix();
}

} // graphics
} // avalon