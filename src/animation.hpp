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

  idle -idle-> idle
  idle -walk-> walking
  idle -run-> walking
  walking -idle-> idle
  walking -walk-> walking
  walking -run-> running
  walking -jump-> jumpingForward
  ...etc...

In a sense, there is a directed graph between each animations, and the
"current intent" chooses the best possible path.

We can make it choose best possible destination and run path-finding to it,
but that's completely unnecessary - we can just build each intent to each path.

However, it's not really easy to make each intent clear - there would be various
conditions that trigger certain animation, etc. But this mechanism needs some
kind of scripting, which isn't really necessary at this point.
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
