#include "FsIOHelper.h"

#include <system_error>

#include "GlobalFsIOHelper.h"

void FsIOHelper::open(const std::string& path, std::ios_base::open_mode mode) {
  (void)mode;

  auto& gfsioh = GlobalFsIOHelper::instance();
  try {
    auto const& cbs = gfsioh.getCallback(path);
    _openCallback = std::get<0>(cbs);
    _closeCallback = std::get<1>(cbs);

    _open = true;
    _openCallback(*this);
  } catch (std::out_of_range const& e) {
      throw std::system_error(EACCES, std::system_category());
  }
}

FsIOHelper::~FsIOHelper() {
    if (is_open())
        close();
}


void FsIOHelper::close() {
  _open = false;
  if (is_open()) {
    _closeCallback(*this);
  }
  str("");
}
