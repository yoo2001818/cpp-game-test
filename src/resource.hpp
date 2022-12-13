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

    resource_entry() = default;
    resource_entry(const resource_entry<T>&) = default;
    resource_entry(resource_entry<T>&&) = default;
    
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

    void update(const T& value) {
      this->mValue = value;
    };

    void update(const std::function<std::optional<T>()>& load_entry_function) {
      this->mLoadEntryFunction = load_entry_function;
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
    resource_manager() {};

    void insert(std::string name, const T& value) {
      auto [it, is_created] = this->entries.emplace(name, resource_entry<T>({ value }));
      if (!is_created) {
        it->second.update(value);
      }
    };

    void insert(std::string name, const std::function<std::optional<T>()>& load_entry_function) {
      auto [it, is_created] = this->entries.emplace(name, resource_entry<T>({ load_entry_function }));
      if (!is_created) {
        it->second.update(load_entry_function);
      }
    };

    resource_entry<T>& get(std::string name) {
      // Note that this will always give an entry even if the resource is not
      // to be found. This is done to allow lazy initialization, and track
      // used-but-not-available resources.
      // Also note that resource_entry<T> reference is stable - you can call
      // this function and use it as long as you want.
      // (Though we will need some kind of reference count tracking later)

      auto it = this->entries.find(name);
      if (it == this->entries.end()) {
        // Create new empty resource
        auto [new_it, is_created] = this->entries.emplace(name, resource_entry<T>());
        return new_it->second;
      }
      return it->second;
    }

    private:
    std::unordered_map<std::string, resource_entry<T>> entries;
    // TODO: We don't do any cleanups yet, but we'll need it later.
    // std::vector<std::reference_wrapper<resource_entry<T>>> used_entries;
  };
}

#endif // RESOURCE_HPP_
