//
//  make render comp.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 19/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef make_render_comp_hpp
#define make_render_comp_hpp

#include <memory>
#include "render component.hpp"
#include <experimental/string_view>

std::shared_ptr<RenderComponent> makeRenderComp(
  std::experimental::string_view,
  Entity *,
  glm::vec2
);

#endif
