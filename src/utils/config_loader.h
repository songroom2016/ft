// Copyright [2020] <Copyright Kevin, kevin.lau.gd@gmail.com>

#ifndef FT_SRC_UTILS_CONFIG_LOADER_H_
#define FT_SRC_UTILS_CONFIG_LOADER_H_

#include <yaml-cpp/yaml.h>

#include <cassert>
#include <fstream>
#include <string>
#include <vector>

#include "trading_server/common/config.h"
#include "utils/string_utils.h"

namespace ft {

bool LoadConfig(const std::string& file, ft::Config* config);

}  // namespace ft

#endif  // FT_SRC_UTILS_CONFIG_LOADER_H_
