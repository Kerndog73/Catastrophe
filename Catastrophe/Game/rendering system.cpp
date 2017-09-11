//
//  rendering system.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 29/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "rendering system.hpp"

#include "camera.hpp"
#include "entity manager.hpp"
#include "render component.hpp"

void RenderingSystem::add(const EntityID id, const std::shared_ptr<RenderComponent> comp) {
  components.emplace(id, comp);
}

void RenderingSystem::rem(const EntityID id) {
  components.erase(id);
}

void RenderingSystem::update(const float delta) {
  camera.update(delta);
}

void RenderingSystem::render(EntityManager &entityMan, NVGcontext *context) {
  for (auto c = components.cbegin(); c != components.cend(); ++c) {
    const Entity &entity = entityMan.getEntity(c->first);
    const RenderingState &rendering = *entity.latestRenderingState;
    c->second->render(context, rendering);
  }
}

void RenderingSystem::cameraDebugRender(NVGcontext *context) {
  camera.debugRender(context);
}

void RenderingSystem::startMotionTrack(const EntityID entity) {
  auto iter = components.find(entity);
  if (iter == components.end()) {
    throw std::runtime_error("Cannot track entity that doesn't exist");
  }
  const CameraMotionTarget *target = iter->second->getMotionTarget();
  camera.motionTrack.start(target);
}

void RenderingSystem::stopMotionTrack() {
  camera.motionTrack.stop();
}

void RenderingSystem::startZoomTrack(const EntityID entity) {
  auto iter = components.find(entity);
  if (iter == components.end()) {
    throw std::runtime_error("Cannot track entity that doesn't exist");
  }
  const CameraZoomTarget *target = iter->second->getZoomTarget();
  camera.zoomTrack.start(target);
}

void RenderingSystem::stopZoomTrack() {
  camera.zoomTrack.stop();
}

Camera &RenderingSystem::getCamera() {
  return camera;
}
