//
//  player input component.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 29/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef player_input_component_hpp
#define player_input_component_hpp

#include <bitset>
#include <SDL2/SDL_scancode.h>
#include "input component.hpp"

class PlayerInputComponent : public InputComponent {
public:
  explicit PlayerInputComponent(Entity *);
  ~PlayerInputComponent() = default;
  
  void update() override;
  bool handleEvent(SDL_Event) override;

private:
  static constexpr size_t MOVING_LEFT_BIT  = 0;
  static constexpr size_t MOVING_RIGHT_BIT = 1;
  static constexpr size_t JUMPING_BIT      = 2;

  Entity *entity;
  std::bitset<3> flags;
  
  bool handleKeyDown(SDL_Scancode);
  bool handleKeyUp(SDL_Scancode);
};

#endif
