/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once

#include <unordered_map>
#include "common/enforce.h"
#include "common/log.h"
#include "common/variant.h"
#include "framework/framework.pb-c.h"

namespace paddle_mobile {
namespace framework {

class BlockDesc;

class Attribute {
 public:
  /*
   *  PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__INT = 0,
  PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__FLOAT = 1,
  PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__STRING = 2,
  PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__INTS = 3,
  PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__FLOATS = 4,
  PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__STRINGS = 5,
  PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__BOOLEAN = 6,
  PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__BOOLEANS = 7,
  PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__BLOCK = 8,
  PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__LONG = 9
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE)
   *
   * */
  static Attribute GetAttrValue(
      PaddleMobile__Framework__Proto__OpDesc__Attr *attr_desc) {
    //    std::cout << "begin get attr value" << std::endl;
    Attribute attr;
    switch (attr_desc->type) {
      case PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__BOOLEAN: {
        attr.Set<bool>(attr_desc->b);
        break;
      }
      case PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__INT: {
        attr.Set<int>(attr_desc->i);
        break;
      }
      case PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__FLOAT: {
        attr.Set<float>(attr_desc->f);
        break;
      }
      case PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__STRING: {
        attr.Set<std::string>(std::string(attr_desc->s));
        break;
      }
      case PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__BOOLEANS: {
        std::vector<bool> val(attr_desc->n_bools);
        for (int i = 0; i < attr_desc->n_bools; ++i) {
          val[i] = attr_desc->bools[i];
        }
        attr.Set<std::vector<bool>>(val);
        break;
      }
      case PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__INTS: {
        std::vector<int> val(attr_desc->n_ints);
        for (int i = 0; i < attr_desc->n_ints; ++i) {
          val[i] = attr_desc->ints[i];
        }
        attr.Set<std::vector<int>>(val);
        break;
      }
      case PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__FLOATS: {
        std::vector<float> val(attr_desc->n_floats);
        for (int i = 0; i < attr_desc->n_floats; ++i) {
          val[i] = attr_desc->floats[i];
        }
        attr.Set<std::vector<float>>(val);
        break;
      }
      case PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__STRINGS: {
        std::vector<std::string> val(attr_desc->n_strings);
        for (int i = 0; i < attr_desc->n_strings; ++i) {
          val[i] = attr_desc->strings[i];
        }
        attr.Set<std::vector<std::string>>(val);
        break;
      }
      case PADDLE_MOBILE__FRAMEWORK__PROTO__ATTR_TYPE__LONG: {
        attr.Set<int64_t>(attr_desc->l);
        break;
      }
      default:
        PADDLE_MOBILE_THROW_EXCEPTION("attr type not support");
    }
    return attr;
  }

  Attribute() {}
  template <typename T, typename... Args>
  Attribute &Set(Args &&... args) {
    variant_.Set<T>(args...);
    return *this;
  }

  template <typename T>
  T &Get() const {
    return variant_.Get<T>();
  }

  template <typename Vistor>
  static typename Vistor::type_t ApplyVistor(Vistor vistor, Attribute attr) {
    if (attr.variant_.TypeId() == typeid(int).hash_code()) {
      return vistor(attr.variant_.Get<int>());
    } else if (attr.variant_.TypeId() == typeid(float).hash_code()) {
      return vistor(attr.variant_.Get<float>());
    } else if (attr.variant_.TypeId() == typeid(std::string).hash_code()) {
      return vistor(attr.variant_.Get<std::string>());
    } else if (attr.variant_.TypeId() == typeid(std::vector<int>).hash_code()) {
      return vistor(attr.variant_.Get<std::vector<int>>());
    } else if (attr.variant_.TypeId() ==
               typeid(std::vector<float>).hash_code()) {
      return vistor(attr.variant_.Get<std::vector<float>>());
    } else if (attr.variant_.TypeId() ==
               typeid(std::vector<std::string>).hash_code()) {
      return vistor(attr.variant_.Get<std::vector<std::string>>());
    } else if (attr.variant_.TypeId() == typeid(bool).hash_code()) {
      return vistor(attr.variant_.Get<bool>());
    } else if (attr.variant_.TypeId() ==
               typeid(std::vector<bool>).hash_code()) {
      return vistor(attr.variant_.Get<std::vector<bool>>());
    } else if (attr.variant_.TypeId() == typeid(int64_t).hash_code()) {
      return vistor(attr.variant_.Get<int64_t>());
    } else {
      throw std::bad_exception();
    }
  }

 private:
  Variant<int, float, std::string, std::vector<int>, std::vector<float>,
          std::vector<std::string>, bool, std::vector<bool>, BlockDesc *,
          int64_t>
      variant_;
};

using AttributeMap = std::unordered_map<std::string, Attribute>;

class AttrReader {
 public:
  explicit AttrReader(const AttributeMap &attrs) : attrs_(attrs) {}

  template <typename T>
  inline T Get(const std::string &name) const {
    //          PADDLE_ENFORCE(attrs_.count(name) != 0, "%s should
    //          be in
    //          AttributeMap",
    //                         name);
    return ((Attribute)attrs_.at(name)).Get<T>();
  }

 private:
  const AttributeMap &attrs_;
};

Print &operator<<(Print &printer, const Attribute &op_desc);

}  // namespace framework
}  // namespace paddle_mobile
