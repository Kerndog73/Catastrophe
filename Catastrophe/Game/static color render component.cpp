//
//  static color render component.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 29/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "static color render component.hpp"

#include "entity.hpp"
#include "rendering context.hpp"

StaticColorRenderComponent::StaticColorRenderComponent(
  const Color color,
  const float width,
  const float height
) : color(color),
    rect(0.0f, 0.0f, width, height) {}

void StaticColorRenderComponent::update(Entity *entity, const float) {
  const b2Vec2 pos = entity->getPos();
  rect.c = {pos.x, pos.y};
}

void StaticColorRenderComponent::render(RenderingContext &context) {
  context.renderRect(color, makeRectCenterSize(rect.c, rect.s));
}

const CameraTarget *StaticColorRenderComponent::getCameraTarget() const {
  return &rect;
}
