//
//  input system.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 29/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "input system.hpp"

#include <SDL2/SDL_events.h>

void InputSystem::init() {}
void InputSystem::quit() {}

void InputSystem::add(const EntityID id, const std::shared_ptr<InputComponent> comp) {
  components.emplace(id, comp);
}

void InputSystem::rem(const EntityID id) {
  components.erase(id);
}

void InputSystem::update() {
  for (auto c = components.cbegin(); c != components.cend(); ++c) {
    c->second->update();
  }
}

void InputSystem::handleEvent(const SDL_Event event) {
  for (auto c = components.cbegin(); c != components.cend(); ++c) {
    if (c->second->handleEvent(event)) {
      break;
    }
  }
}
