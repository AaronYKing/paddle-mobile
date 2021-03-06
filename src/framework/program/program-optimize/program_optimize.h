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

#include <string>
#include <vector>

#include "framework/operator.h"
#include "framework/program/program-optimize/node.h"
#include "framework/program/program_desc.h"

namespace paddle_mobile {

namespace framework {
class ProgramOptimize {
 public:
  ProgramOptimize() {}
  std::shared_ptr<ProgramDesc> Optimize();
  std::shared_ptr<ProgramDesc> FushionOptimize(
      std::shared_ptr<ProgramDesc> ori_des);

 private:
  //                std::shared_ptr<ProgramDesc> ori_desc_;
  std::vector<std::unordered_map<std::string, std::shared_ptr<Node>>>
      outputs_nodes_;
};
}  // namespace framework
}  // namespace paddle_mobile
