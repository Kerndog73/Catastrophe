//
//  launcher animation component.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 5/10/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "launcher animation component.hpp"

#include "entity.hpp"
#include "transform.hpp"
#include "launcher physics component.hpp"
#include "../Libraries/Box2D/Common/b2Math.h"

void LauncherAnimationComponent::init(const YAML::Node &) {}

void LauncherAnimationComponent::update(float) {
  const auto physics = getExpectedCompImpl<LauncherPhysicsComponent>();
  progress = physics->getRelTranslation();
  Transform transform;
  transform.pos = physics->getCenter();
  //the sprite is pointing in the +y direction but getRotation returns an angle
  //relative to the +x direction so we subtract a quarter turn
  transform.rotation = physics->getRotation() - (b2_pi / 2);
  transform.scale = glm::vec2(physics->getLimitSize());
  model = transform.getMat3();
}

float LauncherAnimationComponent::getProgress() const {
  return progress;
}

glm::mat3 LauncherAnimationComponent::getModelMat() const {
  return model;
}
