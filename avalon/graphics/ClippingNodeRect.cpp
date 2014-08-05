#include "ClippingNodeRect.h"

namespace avalon {
namespace graphics {

void ClippingNodeRect::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
    cocos2d::Director::getInstance()->pushMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

    glEnable(GL_SCISSOR_TEST);

    auto glView = cocos2d::Director::getInstance()->getOpenGLView();
    glView->setScissorInPoints(
        getPositionX(), getPositionY(),
        getContentSize().width, getContentSize().height
    );
    Node::visit();

    glDisable(GL_SCISSOR_TEST);

    cocos2d::Director::getInstance()->popMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

} // graphics
} // avalon