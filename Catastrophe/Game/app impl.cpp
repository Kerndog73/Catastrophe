//
//  app impl.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 9/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app impl.hpp"

#include "player.hpp"
#include "platform.hpp"
#include "file constants.hpp"
#include "window constants.hpp"

std::unique_ptr<AppImpl> app = nullptr;

bool AppImpl::init() {
  SDLApp::initWindow(WINDOW_DESC, WINDOW_VSYNC);
  renderingSystem.init(renderer.get(), SPRITE_SHEET_PATH);
  physicsSystem.init(renderer.get());
  inputSystem.init();
  entityManager.init(inputSystem, physicsSystem, renderingSystem);
  player = entityManager.create(makePlayer, b2Vec2(0.0f, 10.0f));
  platform = entityManager.create(makePlatform,
    Rect({0.0f, -WINDOW_METER_SIZE.y / 2.0f + 1.0f}, {WINDOW_METER_SIZE.x, 2.0f})
  );
  return true;
}

void AppImpl::quit() {
  entityManager.quit();
  inputSystem.quit();
  physicsSystem.quit();
  renderingSystem.quit();
  SDLApp::quitWindow();
}

bool AppImpl::input(uint64_t) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      return false;
    } else {
      inputSystem.handleEvent(e);
    }
  }
  return true;
}

bool AppImpl::update(const uint64_t deltaMS) {
  physicsSystem.update(deltaMS / 1000.0f);
  entityManager.update(deltaMS / 1000.0f);
  return true;
}

void AppImpl::render(const uint64_t) {
  renderer.clear();
  if constexpr (ENABLE_GAME_RENDER) {
    renderingSystem.render();
  }
  if constexpr (ENABLE_DEBUG_PHYSICS_RENDER) {
    physicsSystem.debugRender();
  }
  renderer.present();
}
