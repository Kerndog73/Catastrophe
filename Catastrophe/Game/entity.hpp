//
//  entity.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 28/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef entity_hpp
#define entity_hpp

#include <memory>
#include "entity id.hpp"

class InputComponent;
class PhysicsComponent;
class AnimationComponent;
class RenderComponent;
class ParticleEffectComponent;

class Entity {
public:
  explicit Entity(EntityID);
  
  std::shared_ptr<InputComponent> input;
  std::shared_ptr<PhysicsComponent> physics;
  std::shared_ptr<AnimationComponent> animation;
  std::shared_ptr<RenderComponent> render;
  std::shared_ptr<ParticleEffectComponent> particles;
  
  EntityID getID() const;

private:
  EntityID id;
};

#endif
