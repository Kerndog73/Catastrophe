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
#include "camera constants.hpp"
#include "register collision listeners.hpp"

std::unique_ptr<AppImpl> app = nullptr;

bool AppImpl::init() {
  SDLApp::initWindow(WINDOW_DESC, WINDOW_VSYNC);
  
  renderingSystem.init(renderer.get(), SPRITE_SHEET_PATH);
  renderingSystem.attachCamera(&camera);
  
  physicsSystem.init(renderer.get());
  physicsSystem.attachCamera(&camera);
  registerCollisionListeners(physicsSystem.getContactListener());
  
  inputSystem.init();
  entityManager.init(inputSystem, physicsSystem, renderingSystem);
  
  player = entityManager.create(makePlayer, b2Vec2(WINDOW_METER_SIZE.x / 2.0f, 10.0f));
  platform = entityManager.create(makePlatform,
    Rect({WINDOW_METER_SIZE.x / 2.0f, 1.0f}, {WINDOW_METER_SIZE.x, 2.0f})
  );
  
  renderingSystem.track(player);
  
  return true;
}

void AppImpl::quit() {
  renderingSystem.stopTracking();

  entityManager.quit();
  inputSystem.quit();
  
  physicsSystem.detachCamera();
  physicsSystem.quit();
  
  renderingSystem.detachCamera();
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
  camera.update(deltaMS / 1000.0f);
  physicsSystem.update(deltaMS / 1000.0f);
  entityManager.update(deltaMS / 1000.0f);
  return true;
}

void AppImpl::render(const uint64_t) {
  renderer.clear();
  if constexpr (ENABLE_DEBUG_PHYSICS_RENDER) {
    physicsSystem.debugRender();
  }
  if constexpr (ENABLE_GAME_RENDER) {
    renderingSystem.render();
  }
  renderer.present();
}
