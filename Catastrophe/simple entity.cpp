//
//  simple entity.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 12/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "simple entity.hpp"

SimpleEntity::SimpleEntity(const EntityID id, const Rect rect, const int moveSpeed)
  : Entity(id), freqLimiter(Time::OP_PER_SEC, moveSpeed), rect(rect) {}

void SimpleEntity::startMoving(const Math::Dir dir) {
  if (moving) {
    nextMoveDir = dir;
    hasNextMoveDir = true;
  } else {
    freqLimiter.reset();
    moveDir = dir;
    moving = true;
  }
}

void SimpleEntity::stopMoving() {
  hasNextMoveDir = false;
}

Rect SimpleEntity::getRect() const {
  return rect;
}

bool SimpleEntity::isMoving() const {
  return moving;
}

Math::Dir SimpleEntity::getMotionDir() const {
  return moveDir;
}

float SimpleEntity::getMotionProgress() const {
  if (moving) {
    return static_cast<float>(freqLimiter.getTimeSinceLast()) / freqLimiter.getDuration();
  } else {
    return 0.0f;
  }
}

void SimpleEntity::update(const uint64_t delta) {
  using ToVec = Math::ToVec<Rect::Scalar, Math::Dir::RIGHT, Math::Dir::DOWN>;
  
  freqLimiter.advance(delta);
  
  if (moving && freqLimiter.canDoOverlap()) {
    rect.p += ToVec::conv(moveDir);
    moving = hasNextMoveDir;
    moveDir = nextMoveDir;
  }
}
