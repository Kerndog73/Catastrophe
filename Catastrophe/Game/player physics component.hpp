//
//  player physics component.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 3/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef player_physics_component_hpp
#define player_physics_component_hpp

#include <forward_list>
#include <yaml-cpp/node/node.h>
#include "physics component.hpp"

class PlayerPhysicsComponent final : public PhysicsComponent {
public:
  PlayerPhysicsComponent(const YAML::Node &, const YAML::Node &, PhysicsSystem &);
  
  void preStep(float) override;
  void postStep() override;
  
  void beginContactingGround(b2Body *);
  void endContactingGround(b2Body *);
  
  glm::vec2 getRelVel() const;
  bool onGround() const;

private:
  std::forward_list<b2Body *> groundBodies;
  float timeTillFinishJump = 0.0f;
  float moveForce;
  float airMoveForce;
  float maxMoveSpeed;
  float jumpForce;
  float maxJumpDur;

  void applyMoveForce(float);
  void jump(float);
};

#endif
