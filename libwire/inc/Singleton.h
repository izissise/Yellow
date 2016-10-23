#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

#include <functional>
#include <memory>
#include <utility>


//! @class Singleton
//! @brief A generic singleton to inherit from.
template <typename SingleClass>
class Singleton {
 public:
  Singleton() = default;
  virtual ~Singleton() = default;

  //! Gets the instance of the singleton.
  //! @return the instance of the singleton.
  template <typename IMPL = SingleClass, typename... Args>
  static SingleClass& instance(Args&&... args) {
      if (_instance.get() == nullptr) {
          _instance.reset(new IMPL(std::forward<Args>(args)...));
      }
    return *(_instance.get());
  }

 private:
  static std::unique_ptr<SingleClass> _instance;  //!< Internal instance of the singleton.

  Singleton(const Singleton&) = delete;
  void operator=(const Singleton&) = delete;
};

template <typename T> std::unique_ptr<T> Singleton<T>::_instance = nullptr;

#endif
