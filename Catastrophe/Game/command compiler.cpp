//
//  command compiler.cpp
//  Catastrophe
//
//  Created by Indi Kernick on 24/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "command compiler.hpp"

#include <stack>
#include "draw commands.hpp"
#include "string view literal.hpp"

namespace {
  #define COMMAND(STR, CLASS)                                                 \
    if (parseStr.check(#STR##_sv)) {                                            \
      return std::make_unique<CLASS##Command>();                                \
    } else

  std::unique_ptr<DrawCommand> identifyDrawCommand(Utils::ParseString &parseStr) {
    //render styles
    
    COMMAND(stroke_color, StrokeColor)
    COMMAND(stroke_paint, StrokePaint)
    COMMAND(fill_color, FillColor)
    COMMAND(fill_paint, FillPaint)
    COMMAND(miter_limit, MiterLimit)
    COMMAND(stroke_width, StrokeWidth)
    COMMAND(line_cap, LineCap)
    COMMAND(line_join, LineJoin)
    COMMAND(global_alpha, GlobalAlpha)
    
    //transform
    
    COMMAND(translate, Translate)
    COMMAND(rotate, Rotate)
    COMMAND(skew_x, SkewX)
    COMMAND(skew_y, SkewY)
    COMMAND(scale, Scale)
    
    //paints
    
    COMMAND(linear_gradient, LinearGradient)
    COMMAND(box_gradient, BoxGradient)
    COMMAND(radial_gradient, RadialGradient)
    COMMAND(image_pattern, ImagePattern)
    
    //scissoring
    
    COMMAND(scissor, Scissor)
    COMMAND(intersect_scissor, IntersectScissor)
    COMMAND(reset_scissor, ResetScissor)
    
    //paths
    
    COMMAND(begin_path, BeginPath)
    COMMAND(move_to, MoveTo)
    COMMAND(line_to, LineTo)
    COMMAND(bezier_to, BezierTo)
    COMMAND(quad_to, QuadTo)
    COMMAND(arc_to, ArcTo)
    COMMAND(close_path, ClosePath)
    COMMAND(path_winding, PathWinding)
    COMMAND(arc, Arc)
    COMMAND(rect, Rect)
    COMMAND(rounded_rect, RoundedRect)
    COMMAND(rounded_rect_varying, RoundedRectVarying)
    COMMAND(ellipse, Ellipse)
    COMMAND(circle, Circle)
    COMMAND(fill, Fill)
    COMMAND(stroke, Stroke)
    /* else */ {
      throw DrawCommandError("Invalid command");
    }
  }
  
  std::unique_ptr<NestedDrawCommand> identifyNestedCommand(Utils::ParseString &parseStr) {
    COMMAND(repeat, Repeat)
    /* else */ {
      return nullptr;
    }
  }
  
  #undef COMMAND
}

std::unique_ptr<RootDrawCommand> compileDrawCommands(
  const std::string &string,
  const FrameSize frameSize,
  const Index numImages,
  Index &numPaints,
  const Utils::LineCol<> startPos
) {
  auto root = std::make_unique<RootDrawCommand>();
  Utils::ParseString parseStr(string);
  std::stack<NestedDrawCommand *> nestedCommands;
  nestedCommands.push(root.get());
  
  try {
    while (true) {
      parseStr.skipWhitespace();
      if (parseStr.size() == 0) {
        break;
      }
      if (parseStr.front() == '#') {
        parseStr.skipUntil('\n');
        continue;
      }
      if (nestedCommands.size() > 1 && nestedCommands.top()->close(parseStr)) {
        nestedCommands.pop();
        continue;
      }
      if (std::unique_ptr<NestedDrawCommand> nested = identifyNestedCommand(parseStr)) {
        nested->load(parseStr, frameSize, numImages, numPaints);
        NestedDrawCommand *const nestedPtr = nested.get();
        nestedCommands.top()->pushCommand(std::move(nested));
        nestedCommands.push(nestedPtr);
        continue;
      } else {
        std::unique_ptr<DrawCommand> command = identifyDrawCommand(parseStr);
        command->load(parseStr, frameSize, numImages, numPaints);
        nestedCommands.top()->pushCommand(std::move(command));
      }
    }
  } catch (std::exception &e) {
    throw CommandCompilerError(
      std::string((parseStr.lineCol() + startPos).asStr())
      + " - "
      + e.what()
    );
  }
  
  return root;
}
