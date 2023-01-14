#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#include <map>
#include <list>
#include <string>

/*
Player animation, or any other animation consists of various states.
Each animation has start, loop, end. Basically, the animation is a state machine
that never stops.

When the player performs different action, the state machine will try to divert
to non-default action. For example, if the player tries to jump,
the 'standing' animation may be interrupted, and the state changes to jump
starting animation, flying animation, and so on.
*/

namespace animation {
  class animation_state {
    public:
    std::string name;
    std::map<std::string, std::string> actions;
    float duration;
  };

  class animation_action {
    public:
    std::string name;
  };

  class animation_definition {
    public:
    std::string initial_state;
    std::map<std::string, animation_state> states;
    std::map<std::string, animation_action> actions;
  };

  class animation {
    public:
    animation_state& state;
    float time;
    animation_definition& definition;
  };
}

#endif // ANIMATION_HPP_
