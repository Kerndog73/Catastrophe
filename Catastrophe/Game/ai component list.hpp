//
//  ai component list.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 21/10/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef ai_component_list_hpp
#define ai_component_list_hpp

#include "missile ai component.hpp"
#include <Simpleton/Type List/type.hpp>
#include "ground 0 droid ai component.hpp"
#include "ground 1 droid ai component.hpp"

using AIComps = List::Type<
  Ground0DroidAIComponent,
  Ground1DroidAIComponent,
  MissileAIComponent
>;

#endif
