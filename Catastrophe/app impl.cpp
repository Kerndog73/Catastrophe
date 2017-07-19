//
//  app impl.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 9/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app impl.hpp"

#include "constants.hpp"

std::unique_ptr<AppImpl> app = nullptr;

AppImpl::AppImpl() {}

bool AppImpl::init() {
  SDLApp::initWindow(WINDOW_DESC, true);
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
      
    }
  }
  return true;
}

bool AppImpl::update(const uint64_t) {
  return true;
}

void AppImpl::render(const uint64_t) {
  renderer.clear();
  
  renderer.present();
}
