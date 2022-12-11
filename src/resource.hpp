#ifndef RESOURCE_HPP_
#define RESOURCE_HPP

#include <unordered_map>
#include <vector>
#include <string>

namespace resource {
  class resource_manager {
    public:

    private:
    std::unordered_map<std::string, resource_entry> entries;
    std::vector<resource_entry&> used_entries;
  };

  class resource_entry {

  };
}

#endif // TILE_HPP_
