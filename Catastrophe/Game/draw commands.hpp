//
//  draw commands.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 24/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef draw_commands_hpp
#define draw_commands_hpp

#include <experimental/tuple>
#include "command compiler.hpp"
#include "string view literal.hpp"
#include <Simpleton/Utils/type list.hpp>

class DrawCommandError final : public std::runtime_error {
public:
  explicit DrawCommandError(const char *);
};

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

class PointType {};
class ScalarType {};
class ColorType {};

template <typename ArgType>
struct GetTupleType {
  //enums
  using type = int;
};

template <>
struct GetTupleType<PointType> {
  using type = Index;
};

template <>
struct GetTupleType<ScalarType> {
  using type = Index;
};

template <>
struct GetTupleType<ColorType> {
  using type = Index;
};

template <typename ArgType>
struct GetFunArgType {
  //enums
  using type = std::tuple<int>;
};

template <>
struct GetFunArgType<PointType> {
  using type = std::tuple<float, float>;
};

template <>
struct GetFunArgType<ScalarType> {
  using type = std::tuple<float>;
};

template <>
struct GetFunArgType<ColorType> {
  using type = std::tuple<NVGcolor>;
};

inline std::tuple<int> getFunArg(const int arg) {
  return std::tuple<int>(arg);
}

inline std::tuple<float, float> getFunArg(const glm::vec2 arg) {
  return std::tuple<float, float>(arg.x, arg.y);
}

inline std::tuple<float> getFunArg(const float arg) {
  return std::tuple<float>(arg);
}

inline std::tuple<NVGcolor> getFunArg(const NVGcolor arg) {
  return std::tuple<NVGcolor>(arg);
}

template <typename Tuple>
struct IsTuple : std::false_type {};
template <typename ...Types>
struct IsTuple<std::tuple<Types...>> : std::true_type {};

template <typename T>
auto getElement(T &&element) {
  if constexpr (IsTuple<std::decay_t<T>>::value) {
    return std::forward<T>(element);
  } else {
    return std::forward_as_tuple(element);
  }
}

template <typename Tuple, size_t ...Is>
auto flattenHelper(Tuple &&tuple, std::index_sequence<Is...>) {
  return std::tuple_cat(getElement(std::get<Is>(tuple))...);
}

template <typename Tuple>
auto flatten(Tuple &&tuple) {
  return flattenHelper(tuple, std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>());
}

Index readIndex(ParseString &, Index);
bool isLiteral(ParseString &);
glm::vec2 readPoint(ParseString &);
float readScalar(ParseString &);
NVGcolor readColor(ParseString &);

template <typename FunctionPtr, FunctionPtr FUNCTION, typename List>
class DrawCommandImpl final : public DrawCommand {
public:
  void load(ParseString &string, const FrameSize frame) override {
    Utils::forEachIndex<Utils::listSize<List>>([this, &string, frame] (const auto i) mutable {
      string.skipWhitespace();
      if (string.empty()) {
        throw DrawCommandError("Not enough arguments");
      }
      
      constexpr size_t index = UTILS_VALUE(i);
      using ListType = Utils::AtIndex<List, index>;
      auto &arg = std::get<index>(data);
      using ArgType = std::tuple_element_t<index, decltype(data)>;
      
      if constexpr (std::is_same<ArgType, Index>::value) {
        if constexpr (std::is_same<ListType, PointType>::value) {
          arg = readIndex(string, frame.numPoints);
        } else if constexpr (std::is_same<ListType, ScalarType>::value) {
          arg = readIndex(string, frame.numScalars);
        } else if constexpr (std::is_same<ListType, ColorType>::value) {
          arg = readIndex(string, frame.numColors);
        }
      } else if constexpr (std::is_same<ArgType, int>::value) {
        string.skipWhitespace();
        arg = ParseEnum<ListType>::parse(string);
      }
    });
  }
  
  void draw(NVGcontext *context, const Frame &frame) const override {
    Utils::ListToTuple<Utils::TransformList<List, GetFunArgType>> funArgs;
    constexpr size_t size = std::tuple_size<decltype(funArgs)>::value;
    
    Utils::forEachIndex<size>([this, &frame, &funArgs] (const auto i) {
      constexpr size_t index = UTILS_VALUE(i);
      using ListType = Utils::AtIndex<List, index>;
      const auto dataArg = std::get<index>(data);
      auto &funArg = std::get<index>(funArgs);
      
             if constexpr (std::is_same<ListType, PointType>::value) {
        funArg = getFunArg(frame.points[dataArg]);
      } else if constexpr (std::is_same<ListType, ScalarType>::value) {
        funArg = getFunArg(frame.scalars[dataArg]);
      } else if constexpr (std::is_same<ListType, ColorType>::value) {
        funArg = getFunArg(frame.colors[dataArg]);
      } else {
        funArg = getFunArg(dataArg);
      }
    });
    
    const auto funArgsWithContext = std::tuple_cat(
      std::make_tuple(context),
      flatten(funArgs)
    );
    std::experimental::apply(FUNCTION, funArgsWithContext);
  }

private:
  Utils::ListToTuple<Utils::TransformList<List, GetTupleType>> data;
  Utils::ListToTuple<Utils::TransformList<List, GetFunArgType>> funArgs;
};

#define COMMAND(NAME, FUN, ...)                                                 \
  using NAME##Command = DrawCommandImpl<decltype(&FUN), &FUN, Utils::TypeList<__VA_ARGS__>>

#define COMMAND_NO_ARGS(NAME, FUN)                                              \
  using NAME##Command = DrawCommandImpl<decltype(&FUN), &FUN, Utils::EmptyList>

//render styles

COMMAND(StrokeColor, nvgStrokeColor, ColorType);
COMMAND(FillColor, nvgFillColor, ColorType);
COMMAND(MiterLimit, nvgMiterLimit, ScalarType);
COMMAND(StrokeWidth, nvgStrokeWidth, ScalarType);
COMMAND(LineCap, nvgLineCap, LineCap);
COMMAND(LineJoin, nvgLineJoin, LineJoin);
COMMAND(GlobalAlpha, nvgGlobalAlpha, ScalarType);

//transforms

COMMAND(Translate, nvgTranslate, PointType);
COMMAND(Rotate, nvgRotate, ScalarType);
COMMAND(SkewX, nvgSkewX, ScalarType);
COMMAND(SkewY, nvgSkewY, ScalarType);
COMMAND(Scale, nvgScale, PointType);

//paths

COMMAND_NO_ARGS(BeginPath, nvgBeginPath);
COMMAND(MoveTo, nvgMoveTo, PointType);
COMMAND(LineTo, nvgLineTo, PointType);
COMMAND(BezierTo, nvgBezierTo, PointType, PointType, PointType);
COMMAND(QuadTo, nvgQuadTo, PointType, PointType);
COMMAND(ArcTo, nvgArcTo, PointType, PointType, ScalarType);
COMMAND_NO_ARGS(ClosePath, nvgClosePath);
COMMAND(PathWinding, nvgPathWinding, NVGwinding);
COMMAND(Arc, nvgArc, PointType, ScalarType, ScalarType, ScalarType, NVGwinding);
COMMAND(Rect, nvgRect, PointType, PointType);
COMMAND(RoundedRect, nvgRoundedRect, PointType, PointType, ScalarType);
COMMAND(RoundedRectVarying, nvgRoundedRectVarying, PointType, PointType, ScalarType, ScalarType, ScalarType, ScalarType);
COMMAND(Ellipse, nvgEllipse, PointType, PointType);
COMMAND(Circle, nvgCircle, PointType, ScalarType);
COMMAND_NO_ARGS(Fill, nvgFill);
COMMAND_NO_ARGS(Stroke, nvgStroke);

#undef COMMAND
#undef COMMAND_NO_ARGS

#endif
