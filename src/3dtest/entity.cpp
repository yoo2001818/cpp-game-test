#include "entity.hpp"

bool operator==(const entity_id &e1, const entity_id &e2) {
  return e1.id == e2.id && e1.version == e2.version;
}

bool operator!=(const entity_id &e1, const entity_id &e2) {
  return e1.id != e2.id || e1.version != e2.version;
}

bool operator<(const entity_id &e1, const entity_id &e2) {
  if (e1.id == e2.id)
    return e1.version < e2.version;
  return e1.id < e2.id;
}

bool operator>(const entity_id &e1, const entity_id &e2) {
  if (e1.id == e2.id)
    return e1.version > e2.version;
  return e1.id > e2.id;
}

bool operator<=(const entity_id &e1, const entity_id &e2) {
  if (e1.id == e2.id)
    return e1.version <= e2.version;
  return e1.id < e2.id;
}

bool operator>=(const entity_id &e1, const entity_id &e2) {
  if (e1.id == e2.id)
    return e1.version >= e2.version;
  return e1.id > e2.id;
}
