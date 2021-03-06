//
//  basic rendering component.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 3/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "basic rendering component.hpp"

#include "entity.hpp"
#include "nvg helper.hpp"
#include "layer names.hpp"
#include "physics component.hpp"
#include "animation component.hpp"

void BasicRenderingComponent::init(RenderingContext &, const YAML::Node &node) {
  layer = 0;
  if (const YAML::Node &layerNode = node["layer"]) {
    layer = getLayerIndex(layerNode.Scalar());
  }
}

AABB BasicRenderingComponent::getAABB() const {
  if (const auto physicsComp = getComp<PhysicsComponent>()) {
    return physicsComp->getAABB();
  } else if (const auto animComp = getComp<AnimationComponent>()) {
    const glm::mat3 modelMat = animComp->getModelMat();
    Math::RectCS<float> rectcs;
    rectcs.center = modelMat[2];
    rectcs.halfSize = {modelMat[0][0] / 2.0f, modelMat[1][1] / 2.0f};
    return static_cast<AABB>(rectcs);
  } else {
    return {};
  }
}

size_t BasicRenderingComponent::getLayer() const {
  return layer;
}

void BasicRenderingComponent::setModelTransform(NVGcontext *const ctx) {
  nvgTransform(ctx, getExpectedComp<AnimationComponent>()->getModelMat());
}
