//
//  static color render component.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 29/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef static_color_render_component_hpp
#define static_color_render_component_hpp

#include <nanovg/nanovg.h>
#include "render component.hpp"

class StaticColorRenderComponent final : public RenderComponent {
public:
  StaticColorRenderComponent(NVGcolor, float, float);
  ~StaticColorRenderComponent() = default;
  
  void update(Entity *, float) override;
  void render(NVGcontext *) override;
  const CameraMotionTarget *getCameraTarget() const override;
  
private:
  NVGcolor color;
  CameraMotionTarget rect;
};

#endif
