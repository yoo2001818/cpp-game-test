#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#include <map>
#include <list>
#include <string>
#include <functional>

#include "game.hpp"
#include "entity.hpp"

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

So, at this point, the game logic itself should provide a list of current
intent, and that alone should determine the animation state.

Furthermore, each "action" should define if it should interrupt current state,
however I'm not sure if it should be done programtically, or automatically.
*/

namespace animation {
  class animation_edge {
    public:
    std::string nextState;
    bool isInterruptable;

    animation_edge(std::string& pNextState, bool pIsInterruptable):
      nextState(pNextState),
      isInterruptable(pIsInterruptable) {};
  };

  class animation_state {
    public:
    std::string name;
    std::map<std::string, animation_edge> edges;
    float duration;
    std::function<void(entity, float)> updateFunction;

    void addEdge(std::string& actionName, std::string& nextState, bool isInterruptable);
    void removeEdge(const std::string& actionName);
  };

  class animation_definition {
    public:
    std::string initialState;
    std::map<std::string, animation_state> states;
    std::list<std::string> actions;

    void addState(animation_state& state);
    void removeState(const std::string& name);
  };

  class animation {
    public:
    animation_state& state;
    float time;
    animation_definition& definition;
  };

  void updateAnimation(game& game);
}

#endif // ANIMATION_HPP_
