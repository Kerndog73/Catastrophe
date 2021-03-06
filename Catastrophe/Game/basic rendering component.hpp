//
//  basic rendering component.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 3/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef basic_rendering_component_hpp
#define basic_rendering_component_hpp

#include "rendering component.hpp"

class BasicRenderingComponent : public RenderingComponent {
public:
  BasicRenderingComponent() = default;
  
  void init(RenderingContext &, const YAML::Node &) override;
  void preRender() override {}
  AABB getAABB() const override;
  size_t getLayer() const override;

protected:
  void setModelTransform(NVGcontext *);

private:
  size_t layer;
};

#endif
