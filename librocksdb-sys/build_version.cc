// Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.

#include <memory>

#include "rocksdb/version.h"
#include "rocksdb/utilities/object_registry.h"
#include "util/string_util.h"

// The build script may replace these values with real values based
// on whether or not GIT is available and the platform settings
const char* rocksdb_build_git_sha = "rocksdb_build_git_sha:641222c71e54eca0cad560956469e6fe76c2914a";
const char* rocksdb_build_git_tag = "rocksdb_build_git_tag:sideplugin-8.04.2-2023-06-20-2926e071";
const char* rocksdb_build_git_date = "rocksdb_build_date:2024-09-02 11:57:55";
#define HAS_GIT_CHANGES 0
#if HAS_GIT_CHANGES == 0
// If HAS_GIT_CHANGES is 0, the GIT date is used.
// Use the time the branch/tag was last modified
const char* rocksdb_build_date = "rocksdb_build_date:2024-09-02 11:57:55";
#else
// If HAS_GIT_CHANGES is > 0, the branch/tag has modifications.
// Use the time the build was created.
const char* rocksdb_build_date = "rocksdb_build_date:2024-09-02 11:57:55";
#endif

std::unordered_map<std::string, ROCKSDB_NAMESPACE::RegistrarFunc> ROCKSDB_NAMESPACE::ObjectRegistry::builtins_ = {};

extern "C" bool RocksDbIOUringEnable() {
  return true;
}

namespace ROCKSDB_NAMESPACE {
static void AddProperty(std::unordered_map<std::string, std::string> *props, const std::string& name) {
  size_t colon = name.find(":");
  if (colon != std::string::npos && colon > 0 && colon < name.length() - 1) {
    // If we found a "@:", then this property was a build-time substitution that failed.  Skip it
    size_t at = name.find("@", colon);
    if (at != colon + 1) {
      // Everything before the colon is the name, after is the value
      (*props)[name.substr(0, colon)] = name.substr(colon + 1);
    }
  }
}

static std::unordered_map<std::string, std::string>* LoadPropertiesSet() {
  auto * properties = new std::unordered_map<std::string, std::string>();
  AddProperty(properties, rocksdb_build_git_sha);
  AddProperty(properties, rocksdb_build_git_tag);
  AddProperty(properties, rocksdb_build_date);
  return properties;
}

const std::unordered_map<std::string, std::string>& GetRocksBuildProperties() {
  static std::unique_ptr<std::unordered_map<std::string, std::string>> props(LoadPropertiesSet());
  return *props;
}

std::string GetRocksVersionAsString(bool with_patch) {
  std::string version = std::to_string(ROCKSDB_MAJOR) + "." + std::to_string(ROCKSDB_MINOR);
  if (with_patch) {
    return version + "." + std::to_string(ROCKSDB_PATCH);
  } else {
    return version;
 }
}

std::string GetRocksBuildInfoAsString(const std::string& program, bool verbose) {
  std::string info = program + " (RocksDB) " + GetRocksVersionAsString(true);
  if (verbose) {
    for (const auto& it : GetRocksBuildProperties()) {
      info.append("\n    ");
      info.append(it.first);
      info.append(": ");
      info.append(it.second);
    }
  }
  return info;
}
} // namespace ROCKSDB_NAMESPACE
