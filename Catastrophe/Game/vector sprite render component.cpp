//
//  vector sprite render component.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 3/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "vector sprite render component.hpp"

#include "vector render.hpp"
#include "vector rendering state.hpp"

VectorRenderComponent::VectorRenderComponent(const float width, const float height)
  : rect(std::make_shared<CameraMotionTarget>(glm::vec2(), glm::vec2(width, height))),
    size(std::make_shared<CameraZoomTarget>(width, height)) {}
    
void VectorRenderComponent::render(NVGcontext *context, const RenderingState &rendering) {
  rect->c = rendering.modelMat[2];
  
  const VectorRenderingState &vectorRender = dynamic_cast<const VectorRenderingState &>(rendering);
  renderSprite(context, vectorRender.shapes, vectorRender.frame, vectorRender.modelMat);
}

std::shared_ptr<const CameraMotionTarget> VectorRenderComponent::getMotionTarget() const {
  return rect;
}

std::shared_ptr<const CameraZoomTarget> VectorRenderComponent::getZoomTarget() const {
  return size;
}

Rect VectorRenderComponent::getAABB(const RenderingState &) const {
  return static_cast<Rect>(*rect);
}
