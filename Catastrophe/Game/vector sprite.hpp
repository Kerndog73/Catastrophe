//
//  vector sprite.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 2/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef vector_sprite_hpp
#define vector_sprite_hpp

#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <unordered_map>
#include <nanovg/nanovg.h>
#include "parse string.hpp"

using TimeSec = float;
using Coord = float;
using Point = glm::tvec2<Coord>;
using Points = std::vector<Point>;
using Colors = std::vector<NVGcolor>;
using Scalars = std::vector<Coord>;

using Index = uint32_t;
using Indicies = std::vector<Index>;
constexpr Index NULL_INDEX = std::numeric_limits<Index>::max();

///The points, colors and scalars for point in time. Values are interpolated
///between keyframes.
struct Frame {
  Points points;
  Colors colors;
  Scalars scalars;
};

struct FrameSize {
  Index numPoints = NULL_INDEX;
  Index numColors = NULL_INDEX;
  Index numScalars = NULL_INDEX;
};

struct PointKeyframe {
  TimeSec offsetSec;
  Points data;
};
using PointKeyframes = std::vector<PointKeyframe>;

struct ColorKeyframe {
  TimeSec offsetSec;
  Colors data;
};
using ColorKeyframes = std::vector<ColorKeyframe>;

struct ScalarKeyframe {
  TimeSec offsetSec;
  Scalars data;
};
using ScalarKeyframes = std::vector<ScalarKeyframe>;

using MetaData = std::unordered_map<std::string, Coord>;
struct Animation {
  TimeSec durationSec;
  PointKeyframes pointFrames;
  ColorKeyframes colorFrames;
  ScalarKeyframes scalarFrames;
  MetaData meta;
};
using Animations = std::unordered_map<std::string, Animation>;

class DrawCommand {
public:
  DrawCommand() = default;
  virtual ~DrawCommand() = default;
  
  virtual void load(ParseString &, FrameSize) = 0;
  virtual void draw(NVGcontext *, const Frame &) const = 0;
};
using DrawCommands = std::vector<std::unique_ptr<DrawCommand>>;

struct Sprite {
  Sprite() = default;
  Sprite(Sprite &&) = default;
  Sprite &operator=(Sprite &&) = default;
  
  Sprite(Animations &&animations, DrawCommands &&drawCommands)
    : animations(std::move(animations)), drawCommands(std::move(drawCommands)) {}
  
  Animations animations;
  DrawCommands drawCommands;
};

#endif
