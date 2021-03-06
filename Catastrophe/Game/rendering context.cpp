//
//  rendering context.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 4/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "rendering context.hpp"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg.hpp"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#include <nanovg/nanovg_gl.h>
#pragma clang diagnostic pop

#include "nvg helper.hpp"
#include "window constants.hpp"
#include <Simpleton/SDL/error.hpp>
#include <Simpleton/Utils/profiler.hpp>

constexpr int DEPTH_BITS = 16;
constexpr int STENCIL_BITS = 8;
constexpr int COLOR_BITS = 32;

namespace {
  const char *glErrorString(const GLenum error) {
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-declarations"
    return reinterpret_cast<const char *>(gluErrorString(error));
    #pragma clang diagnostic pop
  }
}

void RenderingContext::init(SDL_Window *newWindow) {
  PROFILE(RenderingContext init);

  window = newWindow;
  
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEPTH_BITS);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, STENCIL_BITS);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, COLOR_BITS);
  
  sdlGLContext = SDL_GL_CreateContext(window);
  if (sdlGLContext == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }
  SDL_GL_SetSwapInterval(WINDOW_VSYNC);
  
  glewExperimental = GL_TRUE;
  const GLenum glewError = glewInit();
  if (glewError != GLEW_OK) {
    throw std::runtime_error(reinterpret_cast<const char *>(glewGetErrorString(glewError)));
  }
  
  const GLenum glError = glGetError();
  if (glError != GL_NO_ERROR) {
    throw std::runtime_error(glErrorString(glError));
  }
  
  int nvgCreateFlags = NVG_ANTIALIAS | NVG_STENCIL_STROKES;
  #ifndef NDEBUG
  nvgCreateFlags |= NVG_DEBUG;
  #endif
  context = nvgCreateGL3(nvgCreateFlags);
  if (context == nullptr) {
    throw std::runtime_error("NanoVG init failed");
  }
  
  renderingResources.init(context);
  fpsFontHandle = renderingResources.getFont("Consolas.ttf");
  
  fpsCounter.init();
}

void RenderingContext::quit() {
  PROFILE(RenderingContext quit);

  renderingResources.quit();
  nvgDeleteGL3(context);
  context = nullptr;
  SDL_GL_DeleteContext(sdlGLContext);
  sdlGLContext = nullptr;
  window = nullptr;
}

void RenderingContext::preRender(const glm::mat3 viewProj) {
  PROFILE(RenderingContext preRender);

  int windowWidth, windowHeight;
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);
  SDL_GL_GetDrawableSize(window, &renderSize.x, &renderSize.y);
  
  glViewport(0, 0, windowWidth, windowHeight);
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(0.0);
  glClearStencil(0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
  const float devicePixelRatio = static_cast<float>(renderSize.x) / windowWidth;
  nvgBeginFrame(context, windowWidth, windowHeight, devicePixelRatio);
  
  nvgReset(context);
  nvgTransform(context, viewProj);
}

void RenderingContext::postRender(
  const bool printFPS,
  uint8_t *const data,
  const size_t size
) {
  PROFILE(RenderingContext postRender);
  
  fpsCounter.frame();
  
  if (printFPS) {
    renderFPS();
  }
  
  nvgEndFrame(context);
  
  if (data != nullptr && static_cast<int>(size) >= renderSize.x * renderSize.y * 4) {
    captureFrame(data);
  }
  
  SDL_GL_SwapWindow(window);
}

RenderingResources &RenderingContext::getResources() {
  return renderingResources;
}

NVGcontext *RenderingContext::getContext() const {
  return context;
}

SDL_Window *RenderingContext::getWindow() const {
  return window;
}

glm::ivec2 RenderingContext::getFramebufferSize() const {
  return renderSize;
}

void RenderingContext::renderFPS() {
  PROFILE(RenderingContext renderFPS);

  //@TODO use to_chars
  const std::string fpsStr = "FPS: " + std::to_string(fpsCounter.get());
  nvgResetTransform(context);
  nvgFontFaceId(context, fpsFontHandle->id);
  nvgFontSize(context, 32.0f);
  nvgFillColor(context, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
  nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
  nvgText(context, 0.0f, 0.0f, fpsStr.c_str(), fpsStr.c_str() + fpsStr.size());
}

void RenderingContext::captureFrame(uint8_t *const data) {
  PROFILE(RenderingContext captureFrame);
  
  while (glGetError() != GL_NO_ERROR);
  glReadPixels(
    0, 0,
    renderSize.x, renderSize.y,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    data
  );
  const GLenum error = glGetError();
  
  if (error != GL_NO_ERROR) {
    throw std::runtime_error(
      std::string("Failed to copy framebuffer into RAM: ") +
      glErrorString(error)
    );
  }
}
