//
//  gun.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 22/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef gun_hpp
#define gun_hpp

#include "gun spec.hpp"
#include <Simpleton/Time/freq limiter.hpp>

class Gun {
public:
  explicit Gun(GunSpec);

  void update(float);

  void reload();
  
  void startFiring();
  unsigned canFire();
  void stopFiring();
  
  void collectClips(unsigned);
  
  float getBulletSpeed() const;
  unsigned getDamage() const;

private:
  Time::DeltaFreqLimiter<float> rof;
  GunSpec spec;
  bool firing = false;
  //for non-automatic weapons
  bool fired = false;
  unsigned clips = 0;
  unsigned bullets = 0;
  //time until player finishs reloading 
  float timeTillReload = 0.0f;
};

#endif
