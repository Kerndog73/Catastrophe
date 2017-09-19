//
//  vector sprite render component.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 3/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef vector_sprite_render_component_hpp
#define vector_sprite_render_component_hpp

#include "render component.hpp"

class VectorRenderComponent final : public RenderComponent {
public:
  VectorRenderComponent(Entity *, float, float);
  
  void render(NVGcontext *) override;
  CameraMotionTargetCPtr getMotionTarget() const override;
  CameraZoomTargetCPtr getZoomTarget() const override;
  Rect getAABB() const override;

private:
  CameraMotionTargetPtr rect;
  CameraZoomTargetPtr size;
};

#endif
