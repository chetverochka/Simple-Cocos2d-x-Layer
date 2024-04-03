#pragma once
#include "cocos2d.h"

class DefaultLayer : public cocos2d::Node {
protected:
	float fadingDuration;
	cocos2d::DrawNode* rectangle;
	cocos2d::EventListenerTouchOneByOne* rectListener;

	virtual void deleteLayer() {
		if (this->getParent()) {
			this->getParent()->removeChild(this);
		}
	}

	bool init() {
		if (!cocos2d::Node::create()) {
			return false;
		}

		fadingDuration = 0.2f;
		this->setContentSize(cocos2d::Director::getInstance()->getVisibleSize());
		this->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));

		initRectangle();
		return true;
	}
	void initRectangle() {
		cocos2d::Size vs = cocos2d::Director::getInstance()->getVisibleSize();
		rectangle = cocos2d::DrawNode::create();

		cocos2d::Vec2 rects[4];
		rects[0] = cocos2d::Vec2(0, 0);
		rects[1] = cocos2d::Vec2(0, vs.height);
		rects[2] = cocos2d::Vec2(vs.width, vs.height);
		rects[3] = cocos2d::Vec2(vs.width, 0);

		rectangle->drawPolygon(rects, 4, cocos2d::Color4F(0, 0, 0, 100), 0, cocos2d::Color4F::BLACK);
		rectangle->setContentSize(this->getContentSize());
		rectangle->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
		rectangle->setPosition(cocos2d::Vec2(this->getContentSize() / 2));
		

		rectListener = cocos2d::EventListenerTouchOneByOne::create();
		rectListener->setSwallowTouches(true);
		rectListener->onTouchBegan = ([this](cocos2d::Touch* t, cocos2d::Event* e) { return true; });
		rectListener->onTouchEnded = ([this](cocos2d::Touch* t, cocos2d::Event* e) { return true; });

		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(rectListener, rectangle);

		cocos2d::FadeOut* fadeOut = cocos2d::FadeOut::create(0);
		cocos2d::FadeIn* fadeIn = cocos2d::FadeIn::create(fadingDuration);
		cocos2d::Sequence* seq = cocos2d::Sequence::create(fadeOut, fadeIn, nullptr);

		rectangle->runAction(seq);

		this->addChild(rectangle, -1);
		
	}
public:
	static DefaultLayer* create() {
		DefaultLayer* layer = new(std::nothrow) DefaultLayer;

		if (layer && layer->init()) {
			layer->autorelease();

			return layer;
		}
		else {
			CC_SAFE_DELETE(layer);
			return nullptr;
		}
	}

	int getMaxOrder(cocos2d::Node* node) {
		std::vector<int> orders;

		for (int i = 0; i < node->getChildrenCount(); i++) {
			orders.push_back(node->getChildren().at(i)->getLocalZOrder());
		}

		std::sort(orders.begin(), orders.end());

		return orders[orders.size() - 1];
	}

	void setParent(cocos2d::Node* parent) override {
		cocos2d::Node::setParent(parent);
		
		if (parent) {
			this->setPosition(cocos2d::Vec2(parent->getContentSize() / 2));
			this->setLocalZOrder(getMaxOrder(parent));
		}
	}
};
