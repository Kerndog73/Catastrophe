//
//  simple entity view.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 12/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "simple entity view.hpp"

SimpleEntityView::SimpleEntityView(Entity *entity, const std::string &name)
  : LocalEntityView(entity), spriteName(name) {}

void SimpleEntityView::render(RenderingContext &ctx, uint64_t) {
  ctx.renderSprite(spriteName, getEntity().getRect());
}
