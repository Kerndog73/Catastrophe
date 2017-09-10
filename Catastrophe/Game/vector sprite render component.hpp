//
//  vector sprite render component.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 3/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef vector_sprite_render_component_hpp
#define vector_sprite_render_component_hpp

#include "vector sprite.hpp"
#include "render component.hpp"

class VectorRenderComponent final : public RenderComponent {
public:
  VectorRenderComponent(float, float);
  
  void render(NVGcontext *, const RenderingState &) override;
  const CameraMotionTarget *getCameraTarget() const override;

private:
  CameraMotionTarget rect;
};

#endif
