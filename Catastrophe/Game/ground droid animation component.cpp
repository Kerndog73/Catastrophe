//
//  ground droid animation component.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 21/10/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "ground droid animation component.hpp"

#include "yaml helper.hpp"
#include "ground droid physics component.hpp"

void GroundDroidAnimationComponent::init(const YAML::Node &node) {
  transform = node.as<Transform>();
  getOptional(bopSpeed, node, "bop speed");
}

void GroundDroidAnimationComponent::update(const float delta) {
  const auto physicsComp = getExpectedCompImpl<const GroundDroidPhysicsComponent>();
  const float velX = physicsComp->getVelX();
  progress += velX * bopSpeed * delta;
  progress = std::fmod(progress, 1.0f);
  
  transform.pos = physicsComp->getPos();
  mat = transform.getMat3();
  dirX.getDir(velX);
}

float GroundDroidAnimationComponent::getProgress() const {
  return progress;
}

glm::mat3 GroundDroidAnimationComponent::getModelMat() const {
  return mat;
}

float GroundDroidAnimationComponent::getDir() const {
  return dirX.getDir();
}
