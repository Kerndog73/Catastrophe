//
//  camera zoom track.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 30/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef camera_zoom_track_hpp
#define camera_zoom_track_hpp

#include <memory>
#include <glm/vec2.hpp>

using CameraZoomTarget = glm::vec2;

struct CameraProps;
extern "C" struct NVGcontext;

//modifies zoomTarget to keep the CameraZoomTarget size within the bounds.
//bounds are in relative to the size of the window
class CameraZoomTrack {
public:
  CameraZoomTrack();
  
  void start(std::shared_ptr<const CameraZoomTarget>);
  void stop();
  
  void setLocal(CameraZoomTarget);
  void startLocal();
  void setAndStartLocal(CameraZoomTarget);
  
  void setMinMaxSize(float);
  void setMinMaxSize(float, float);
  
  float calcZoomTarget(CameraProps) const;
  
  void debugRender(NVGcontext *, CameraProps) const;

private:
  std::weak_ptr<const CameraZoomTarget> target;
  std::shared_ptr<CameraZoomTarget> localTarget;
  float minSize;
  float maxSize;
};

#endif
