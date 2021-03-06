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

#include <vector>

#include "common/types.h"
#include "framework/framework.pb-c.h"
#include "framework/paddle_mobile_object.h"
#include "framework/program/block_desc.h"

namespace paddle_mobile {
namespace framework {

class ProgramDesc : PaddleMobileObject {
 public:
  friend class Node;
  friend class ProgramOptimize;
  explicit ProgramDesc(PaddleMobile__Framework__Proto__ProgramDesc *desc);

  std::shared_ptr<BlockDesc> Block(size_t idx);

  const std::vector<std::shared_ptr<BlockDesc>> &Blocks() { return blocks_; }
  ProgramDesc(const ProgramDesc &program_desc) {
    for (auto &block : program_desc.blocks_) {
      std::shared_ptr<BlockDesc> copy_block =
          std::make_shared<BlockDesc>(*block);
      blocks_.push_back(copy_block);
    }
  }

  void Description(std::string header = "");

 private:
  std::vector<std::shared_ptr<BlockDesc>> blocks_;
};

}  // namespace framework
}  // namespace paddle_mobile
