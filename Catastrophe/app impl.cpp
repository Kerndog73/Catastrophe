//
//  app impl.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 9/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app impl.hpp"

#include "window constants.hpp"
#include "handle player input.hpp"

std::unique_ptr<AppImpl> app = nullptr;

bool AppImpl::init() {
  SDLApp::initWindow(WINDOW_DESC, WINDOW_VSYNC);
  SDL_RenderSetLogicalSize(renderer.get(), WINDOW_PIXEL_SIZE.x, WINDOW_PIXEL_SIZE.y);
  return true;
}

void AppImpl::quit() {
  SDLApp::quitWindow();
}

bool AppImpl::input(uint64_t) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      return false;
    } else {
      handlePlayerInput(player, e);
    }
  }
  return true;
}

bool AppImpl::update(const uint64_t deltaMS) {
  player.update(deltaMS / 1000.0f);
  return true;
}

void AppImpl::render(const uint64_t) {
  renderer.clear();
  player.render(renderer.get());
  renderer.present();
}