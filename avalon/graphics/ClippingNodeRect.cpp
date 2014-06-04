#include "ClippingNodeRect.h"

namespace avalon {
namespace graphics {

void ClippingNodeRect::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated)
{
    cocos2d::kmGLPushMatrix();
    glEnable(GL_SCISSOR_TEST);


    auto glView = cocos2d::Director::getInstance()->getOpenGLView();
    glView->setScissorInPoints(
        getPositionX(), getPositionY(),
        getContentSize().width, getContentSize().height
    );
    Node::visit();

    glDisable(GL_SCISSOR_TEST);
    cocos2d::kmGLPopMatrix();
}

} // graphics
} // avalon