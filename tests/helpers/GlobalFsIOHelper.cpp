#include "GlobalFsIOHelper.h"

void GlobalFsIOHelper::registerPath(std::string const& path,
                                    std::function<void(std::stringstream& io)> openCb,
                                    std::function<void(std::stringstream const& io)> closeCb) {
    _callbackMap[path] = std::make_tuple(openCb, closeCb);
}

void GlobalFsIOHelper::unregisterPath(std::string const& path) {
  _callbackMap.erase(path);
}

void GlobalFsIOHelper::unregisterAll() {
  _callbackMap.clear();
}

std::tuple<std::function<void (std::stringstream &)>, std::function<void (const std::stringstream &)>> const& GlobalFsIOHelper::getCallback(const std::string& path) const
{
    return _callbackMap.at(path);
}
