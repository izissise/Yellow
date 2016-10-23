#ifndef GLOBALFSIOHELPER_H
#define GLOBALFSIOHELPER_H

#include <functional>
#include <map>
#include <tuple>
#include <string>
#include <sstream>

#include "Singleton.h"

class GlobalFsIOHelper : public Singleton<GlobalFsIOHelper> {
public:
    GlobalFsIOHelper() = default;
    virtual ~GlobalFsIOHelper() = default;

    void registerPath(std::string const& path,
                      std::function<void(std::stringstream& io)> openCb,
                      std::function<void(std::stringstream const& io)> closeCb);
    void unregisterPath(std::string const& path);
    void unregisterAll();

    std::tuple<std::function<void(std::stringstream& io)>, std::function<void(std::stringstream const& io)>> const& getCallback(std::string const& path) const;

private:
    std::map<std::string, std::tuple<std::function<void(std::stringstream& io)>, std::function<void(std::stringstream const& io)>>> _callbackMap;
};

#endif // GLOBALFSIOHELPER_H
