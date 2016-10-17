#ifndef FSIOHELPER_H
#define FSIOHELPER_H

#include <string>
#include <sstream>
#include <ios>
#include <functional>

class FsIOHelper : public std::stringstream {
public:
    FsIOHelper() = default;
    virtual ~FsIOHelper();

    void open(std::string const& path, std::ios_base::open_mode mode);
    void close();

    bool is_open() const { return _open; };
private:
    std::function<void(std::stringstream& io)> _openCallback;
    std::function<void(std::stringstream const& io)> _closeCallback;
    bool _open;
};

#endif // FSIOHELPER_H
