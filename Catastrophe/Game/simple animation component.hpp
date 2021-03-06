//
//  simple animation component.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 17/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef simple_animation_component_hpp
#define simple_animation_component_hpp

#include "transform.hpp"
#include "animation component.hpp"
#include <Simpleton/Time/simple anim.hpp>

class SimpleAnimationComponent final : public AnimationComponent {
public:
  SimpleAnimationComponent() = default;
  
  void init(const YAML::Node &) override;
  void update(float) override;
  float getProgress() const override;
  glm::mat3 getModelMat() const override;

private:
  Transform transform;
  glm::mat3 model;
  Time::SimpleAnim<float> anim;
};

#endif
