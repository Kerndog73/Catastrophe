//
//  player.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 19/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "player.hpp"

#include "window constants.hpp"
#include "player constants.hpp"
#include "dir to vec.hpp"
#include "rendering context.hpp"

Player::Player()
  : rect(0, 0, 1, 1) {}

void Player::startMoving(const Math::Dir dir) {
  moveDir.start(dir);
}

void Player::stopMoving(const Math::Dir dir) {
  moveDir.stop(dir);
}

void Player::update(const float delta) {
  if (moveDir.get() != Math::Dir::NONE) {
    const Rect prevRect = rect;
    rect.p += ToVec::conv(moveDir.get(), delta * PLAYER_MOVE_SPEED);
    onMoveDispatcher.dispatch(prevRect, rect);
  }
}

void Player::render(RenderingContext &renderer) const {
  renderer.renderSprite("rat", rect);
}

void Player::onMove(OnMoveDispatcher::SettableListener listener) {
  onMoveDispatcher.setListener(listener);
  onMoveDispatcher.dispatch(rect, rect);
}

void Player::offMove() {
  onMoveDispatcher.remListener();
}
