//
//  debug draw.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 30/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "debug draw.hpp"

#include <cmath>
#include <glm/vec2.hpp>
#include <nanovg/nanovg.h>
#include "physics constants.hpp"

namespace {
  b2Color brighten(const b2Color color) {
    constexpr float COEF = 1.1f;
    return {
      std::fmin(color.r * COEF, 1.0f),
      std::fmin(color.g * COEF, 1.0f),
      std::fmin(color.b * COEF, 1.0f),
      color.a,
    };
  }
  
  NVGcolor castColor(const b2Color color) {
    return nvgRGBAf(color.r, color.g, color.b, color.a);
  }
}

DebugDraw::DebugDraw()
  : b2Draw() {
  m_drawFlags = 0xFFFFFFFF;
}

void DebugDraw::DrawPolygon(const b2Vec2 *verts, const int32 numVerts, const b2Color &color) {
  if (renderer) {
    if (numVerts == 0) {
      return;
    }
    
    nvgBeginPath(renderer);
    nvgStrokeColor(renderer, castColor(color));
    nvgStrokeWidth(renderer, DEBUG_DRAW_STROKE_WIDTH);
    nvgMoveTo(renderer, verts[0].x, verts[0].y);
    
    const b2Vec2 *const endVerts = verts + numVerts;
    for (const b2Vec2 *v = verts + 1; v != endVerts; ++v) {
      nvgLineTo(renderer, v->x, v->y);
    }
    nvgLineTo(renderer, verts[0].x, verts[0].y);
    
    nvgStroke(renderer);
  }
}

void DebugDraw::DrawSolidPolygon(const b2Vec2 *verts, const int32 numVerts, const b2Color &color) {
  if (renderer) {
    if (numVerts == 0) {
      return;
    }
    
    nvgBeginPath(renderer);
    nvgFillColor(renderer, castColor(color));
    nvgMoveTo(renderer, verts[0].x, verts[0].y);
    
    const b2Vec2 *const endVerts = verts + numVerts;
    for (const b2Vec2 *v = verts + 1; v != endVerts; ++v) {
      nvgLineTo(renderer, v->x, v->y);
    }
    nvgLineTo(renderer, verts[0].x, verts[0].y);
    
    nvgFill(renderer);
  }
}

void DebugDraw::DrawCircle(const b2Vec2 &center, const float32 radius, const b2Color &color) {
  if (renderer) {
    nvgBeginPath(renderer);
    nvgStrokeColor(renderer, castColor(color));
    nvgStrokeWidth(renderer, DEBUG_DRAW_STROKE_WIDTH);
    nvgCircle(renderer, center.x, center.y, radius);
    nvgStroke(renderer);
  }
}

void DebugDraw::DrawSolidCircle(const b2Vec2 &center, const float32 radius, const b2Vec2 &axis, const b2Color &color) {
  DrawPoint(center, radius, color);
  DrawSegment(center, center + radius * axis, brighten(color));
}

void DebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {
  if (renderer) {
    nvgBeginPath(renderer);
    nvgStrokeColor(renderer, castColor(color));
    nvgStrokeWidth(renderer, DEBUG_DRAW_STROKE_WIDTH);
    nvgMoveTo(renderer, p1.x, p1.y);
    nvgLineTo(renderer, p2.x, p2.y);
    nvgStroke(renderer);
  }
}

void DebugDraw::DrawTransform(const b2Transform &) {
  
}

void DebugDraw::DrawPoint(const b2Vec2 &p, const float32 size, const b2Color &color) {
  if (renderer) {
    nvgBeginPath(renderer);
    nvgFillColor(renderer, castColor(color));
    nvgCircle(renderer, p.x, p.y, size);
    nvgFill(renderer);
  }
}

void DebugDraw::attachRenderer(NVGcontext *newRenderer) {
  renderer = newRenderer;
}

void DebugDraw::detachRenderer() {
  renderer = nullptr;
}
