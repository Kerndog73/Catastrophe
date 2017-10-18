//
//  particle system.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 3/10/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef particle_system_hpp
#define particle_system_hpp

#include "particle.hpp"
#include "entity id.hpp"
#include <unordered_map>
#include "render job.hpp"
#include <yaml-cpp/yaml.h>
#include <experimental/optional>
#include "particle group size.hpp"
#include <Simpleton/Memory/block allocator.hpp>

extern "C" struct NVGcontext;
class ParticleComponent;
class RenderManager;

class ParticleSystem {
public:
  static constexpr size_t NUM_GROUPS = 64;
  static constexpr size_t GROUP_SIZE = PARTICLE_GROUP_SIZE;

  using CompPtr = std::shared_ptr<ParticleComponent>;

  ParticleSystem() = default;
  ~ParticleSystem() = default;
  
  void init(RenderManager &);
  void quit();
  
  void add(EntityID, CompPtr, const YAML::Node &);
  void rem(EntityID);
  
  void update(float);
  
private:
  struct CompData {
    CompPtr comp;
    Particle *firstParticle;
  };

  using ParticleAllocator = Memory::BlockAllocator<Particle, GROUP_SIZE>;
  std::experimental::optional<ParticleAllocator> particles;
  RenderManager *renderMan = nullptr;
  
  class Layer final : public RenderJob {
  public:
    Layer() = default;
    
    void render(RenderingContext &) override;
    bool alive() const override;
    
    std::unordered_map<EntityID, CompData> comps;
    bool dead = false;
  };
  
  std::vector<std::shared_ptr<Layer>> layers;
};

#endif
