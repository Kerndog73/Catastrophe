//
//  parse nvg enum.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 28/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef parse_nvg_enum_hpp
#define parse_nvg_enum_hpp

#include <nanovg/nanovg.h>
#include "parse string.hpp"
#include "command errors.hpp"
#include "string view literal.hpp"

//NanoVG doesn't distungish between line cap and line join enum
class LineCap {};
class LineJoin {};

template <typename EnumType>
struct ParseEnum;

template <>
struct ParseEnum<LineCap> {
  static int parse(ParseString &capStr) {
           if (capStr.check("butt"_sv)) {
      return NVG_BUTT;
    } else if (capStr.check("round"_sv)) {
      return NVG_ROUND;
    } else if (capStr.check("square"_sv)) {
      return NVG_SQUARE;
    } else {
      throw DrawCommandError("Invalid line cap");
    }
  }
};

template <>
struct ParseEnum<LineJoin> {
  static int parse(ParseString &joinStr) {
           if (joinStr.check("miter"_sv)) {
      return NVG_MITER;
    } else if (joinStr.check("round"_sv)) {
      return NVG_ROUND;
    } else if (joinStr.check("bevel"_sv)) {
      return NVG_BEVEL;
    } else {
      throw DrawCommandError("Invalid line join");
    }
  }
};

template <>
struct ParseEnum<NVGwinding> {
  static int parse(ParseString &windingStr) {
           if (windingStr.check("ccw"_sv) || windingStr.check("solid"_sv)) {
      return NVG_CCW;
    } else if (windingStr.check("cw"_sv) || windingStr.check("hole"_sv)) {
      return NVG_CW;
    } else {
      throw DrawCommandError("Invalid winding");
    }
  }
};

#endif
