#include "animation.hpp"

void animation::animation_state::addEdge(std::string& actionName, std::string& nextState, bool isInterruptable) {
  animation_edge edge {
    nextState,
    isInterruptable,
  };
  this->edges.insert_or_assign(actionName, edge);
}

void animation::animation_state::removeEdge(const std::string& actionName) {
  this->edges.erase(actionName);
}

void animation::animation_definition::addState(animation_state& state) {
  this->states.insert_or_assign(state.name, state);
}

void animation::animation_definition::removeState(const std::string& name) {
  this->states.erase(name);
}

void animation::updateAnimation(game& game) {
  auto query = game.mWorld.getQuery<animation>();
  for (auto entity : query) {
    auto animation_val = entity->get<animation>();
  }
}