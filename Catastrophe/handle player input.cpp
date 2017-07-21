//
//  handle player input.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 20/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "handle player input.hpp"

#include "input constants.hpp"
#include <SDL2/SDL_events.h>

Math::Dir getMoveDir(const SDL_Scancode key) {
  switch (key) {
    case UP_KEY:
      return Math::Dir::UP;
    case RIGHT_KEY:
      return Math::Dir::RIGHT;
    case DOWN_KEY:
      return Math::Dir::DOWN;
    case LEFT_KEY:
      return Math::Dir::LEFT;
    default:
      return Math::Dir::NONE;
  }
}

Math::Dir getShootDir(const SDL_Scancode key) {
  switch (key) {
    case SHOOT_UP_KEY:
      return Math::Dir::UP;
    case SHOOT_RIGHT_KEY:
      return Math::Dir::RIGHT;
    case SHOOT_DOWN_KEY:
      return Math::Dir::DOWN;
    case SHOOT_LEFT_KEY:
      return Math::Dir::LEFT;
    default:
      return Math::Dir::NONE;
  }
}

void handleKeyDown(Player &player, const SDL_KeyboardEvent keyEvent) {
  assert(keyEvent.type == SDL_KEYDOWN);
  
  if (keyEvent.repeat) {
    return;
  }
  
  const Math::Dir moveDir = getMoveDir(keyEvent.keysym.scancode);
  if (moveDir != Math::Dir::NONE) {
    player.startMoving(moveDir);
  }
  
  const Math::Dir shootDir = getShootDir(keyEvent.keysym.scancode);
  if (shootDir != Math::Dir::NONE) {
    player.startShooting(shootDir);
  }
}

void handleKeyUp(Player &player, const SDL_KeyboardEvent keyEvent) {
  assert(keyEvent.type == SDL_KEYUP);
  
  const Math::Dir moveDir = getMoveDir(keyEvent.keysym.scancode);
  if (moveDir != Math::Dir::NONE) {
    player.stopMoving(moveDir);
  }
  
  const Math::Dir shootDir = getShootDir(keyEvent.keysym.scancode);
  if (shootDir != Math::Dir::NONE) {
    player.stopShooting(shootDir);
  }
}

void handlePlayerInput(Player &player, const SDL_Event e) {
  if (e.type == SDL_KEYDOWN) {
    handleKeyDown(player, e.key);
  } else if (e.type == SDL_KEYUP) {
    handleKeyUp(player, e.key);
  }
}
