//
//  exhaust particle component.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 16/10/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef exhaust_particle_component_hpp
#define exhaust_particle_component_hpp

#include "nanovg.hpp"
#include <glm/vec2.hpp>
#include "particle component.hpp"
#include <Simpleton/Time/freq limiter.hpp>

class ExhaustParticleComponent final : public ParticleComponent {
public:
  ExhaustParticleComponent() = default;
  virtual ~ExhaustParticleComponent() = default;
  
  void init(const YAML::Node &, Particle *) override;
  void move(float, Particle *) override;
  void render(NVGcontext *, const Particle *) override;
  size_t getLayer() const override;

private:
  Time::DeltaFreqLimiter<float> freqLimiter;
  NVGcolor color = {{{0.5f, 0.5f, 0.5f, 0.5f}}};
  glm::vec2 relPos = {0.0f, 0.0f};
  size_t subGroupSize = 32;
  size_t groupSize;
  size_t oldestSubGroupIndex = 0;
  size_t layer = 0;
  float spread = 1.0f;
  float size = 0.1f;
  float lifetime = 1.0f;
  
  void updateTick(Particle *);
};

#endif
