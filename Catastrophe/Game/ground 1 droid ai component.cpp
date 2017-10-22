//
//  ground 1 droid ai component.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 22/10/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "ground 1 droid ai component.hpp"

#include "yaml helper.hpp"
#include "ground droid physics component.hpp"

void Ground1DroidAIComponent::init(const YAML::Node &node) {
  getOptional(lookingDuration, node, "looking duration");
}

void Ground1DroidAIComponent::update(const float delta) {
  const auto physicsComp = getExpectedCompImpl<GroundDroidPhysicsComponent>();
  const bool seePlayer = physicsComp->canSeePlayer();
  const glm::vec2 playerPos = physicsComp->getPlayerPos();
  const glm::vec2 droidPos = physicsComp->getPos();
  
  //@TODO STATE DESIGN PATTERN NOW!!!
  
  if (chasing) {
    if (!seePlayer) {
      chasing = false;
      timeSinceLook = 0.0f;
    } else {
      lookingRight = playerPos.x > droidPos.x;
    }
  } else {
    if (seePlayer) {
      chasing = true;
    } else {
      timeSinceLook += delta;
      if (timeSinceLook >= lookingDuration) {
        timeSinceLook = 0.0f;
        lookingRight = !lookingRight;
      }
    }
  }
}

GroundDroidAIComponent::MoveDir Ground1DroidAIComponent::getMoveDir() const {
  return lookingRight ? MoveDir::RIGHT : MoveDir::LEFT;
}

GroundDroidAIComponent::MoveSpeed Ground1DroidAIComponent::getMoveSpeed() const {
  return chasing ? MoveSpeed::FAST : MoveSpeed::STOP;
}

bool Ground1DroidAIComponent::shouldShoot() const {
  return chasing;
}
