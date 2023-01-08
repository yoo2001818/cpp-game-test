#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#include <map>
#include <list>
#include <string>

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
