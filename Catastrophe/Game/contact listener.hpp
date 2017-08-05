//
//  contact listener.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 5/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef contact_listener_hpp
#define contact_listener_hpp

#include "../Libraries/Box2D/Dynamics/b2WorldCallbacks.h"

class ContactListener final : public b2ContactListener {
public:
  ContactListener() = default;
  ~ContactListener() = default;
  
  void BeginContact(b2Contact *) override;
  void EndContact(b2Contact *) override;
};

#endif
