#include "animation.hpp"

void animation::animation_state::addEdge(std::string& actionName, std::string& nextState, bool isInterruptable) {
  animation_edge edge {
    nextState,
    isInterruptable,
  };
  this->edges.insert_or_assign(actionName, edge);
}

void animation::animation_state::removeEdge(std::string& actionName) {
  this->edges.erase(actionName);
}

void animation::animation_definition::addState(animation_state state) {

}

void animation::animation_definition::removeState(std::string name) {

}

void animation::updateAnimation(game& game) {
  
}
