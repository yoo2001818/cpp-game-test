#ifndef RESOURCE_HPP_
#define RESOURCE_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <optional>

namespace resource {
  template<class T>
  class resource_entry {
    public:
    int mUseCount = 0;
    bool mIsLoadable = false;

    resource_entry() {};
    
    resource_entry(T&& value): mValue(value) {};
    resource_entry(const T& value): mValue(value) {};
    
    resource_entry(const std::function<std::optional<T>()>& load_entry_function):
      mLoadEntryFunction(load_entry_function) {};

    std::optional<T> load() {
      if (this->mIsLoadable && this->mLoadEntryFunction.has_value()) {
        return this->mLoadEntryFunction.value()();
      }
      // This is not a loadable resource
      return std::nullopt;
    };

    void unload() {
      this->mValue = std::nullopt;
    };

    void retry() {
      this->mHasFailed = false;
    };

    std::optional<T>& value() {
      if (!mValue.has_value() && !this->mHasFailed) {
        auto loaded = this->load();
        if (loaded.has_value()) {
          this->mValue = loaded;
        } else {
          this->mHasFailed = true;
        }
      }
      return this->mValue;
    };

    private:
    std::optional<std::function<std::optional<T>()>> mLoadEntryFunction = std::nullopt;
    std::optional<T> mValue = std::nullopt;
    bool mHasFailed = false;
  };

  template<class T>
  class resource_manager {
    public:

    private:
    std::unordered_map<std::string, resource_entry<T>> entries;
    std::vector<std::reference_wrapper<resource_entry<T>>> used_entries;
  };
}

#endif // RESOURCE_HPP_
