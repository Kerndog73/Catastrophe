//
//  spinning blade rendering component.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 8/10/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "spinning blade rendering component.hpp"

#include "entity.hpp"
#include "animation component.hpp"
#include <Simpleton/Math/vectors.hpp>

void SpinningBladeRenderingComponent::render(NVGcontext *const ctx) {
  setModelTransform(ctx);
  
  const float prog = getExpectedCompImpl<AnimationComponent>()->getProgress();
  nvgRotate(ctx, 2.0f * NVG_PI * ROTATIONS_PER_SECOND * prog);
  
  nvgFillColor(ctx, COLOR);
  
  const glm::vec2 pos = Math::angleMag(SPIKE_ANGLE, INNER_RADIUS);
  nvgBeginPath(ctx);
  for (int s = 0; s != NUM_SPIKES; ++s) {
    nvgMoveTo(ctx, pos.x, -pos.y);
    nvgLineTo(ctx, OUTER_RADIUS, 0.0f);
    nvgLineTo(ctx, pos.x, pos.y);
    nvgLineTo(ctx, pos.x, -pos.y);
    nvgRotate(ctx, SPIKE_ANGLE);
  }
  nvgFill(ctx);
  
  nvgBeginPath(ctx);
  nvgCircle(ctx, 0.0f, 0.0f, INNER_RADIUS + 0.05f);
  nvgFill(ctx);
}
