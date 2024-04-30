/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CYLON_SRC_CYLON_CTX_ARROW_MEMORY_POOL_UTILS_HPP_
#define CYLON_SRC_CYLON_CTX_ARROW_MEMORY_POOL_UTILS_HPP_

#include <arrow/memory_pool.h>
#include <cylon/ctx/cylon_context.hpp>

namespace cylon {

inline arrow::Status ArrowStatus(const cylon::Status &status) {
  if (status.is_ok()) {
    return arrow::Status::OK();
  }
  return arrow::Status(static_cast<arrow::StatusCode>(status.get_code()), status.get_msg());
}

class ProxyMemoryPool : public arrow::MemoryPool {
 public:
  explicit ProxyMemoryPool(cylon::MemoryPool *tx_memory) {
    this->tx_memory = tx_memory;
  }

  ~ProxyMemoryPool() override {
    delete tx_memory;
  }

  arrow::Status Allocate(int64_t size, int64_t alignment, uint8_t **out) override {
    return ArrowStatus(tx_memory->Allocate(size, alignment, out));
  }

  arrow::Status Reallocate(int64_t old_size, int64_t new_size, int64_t alignment, uint8_t **ptr) override {
    return ArrowStatus(tx_memory->Reallocate(old_size, new_size, alignment, ptr));
  };

  void Free(uint8_t *buffer, int64_t size, int64_t alignment) override {
    tx_memory->Free(buffer, size, alignment);
  }

  int64_t bytes_allocated() const override {
    return this->tx_memory->bytes_allocated();
  }

  int64_t total_bytes_allocated() const override {
    return this->tx_memory->total_bytes_allocated();
  }

  int64_t num_allocations() const override {
    return this->tx_memory->num_allocations();
  }

  int64_t max_memory() const override {
    return this->tx_memory->max_memory();
  }

  std::string backend_name() const override {
    return this->tx_memory->backend_name();
  }

 private:
  cylon::MemoryPool *tx_memory;
};

arrow::MemoryPool *ToArrowPool(const std::shared_ptr<cylon::CylonContext> &ctx);
arrow::MemoryPool *ToArrowPool(cylon::CylonContext* ctx);
arrow::MemoryPool *ToArrowPool(MemoryPool* pool);
}

#endif //CYLON_SRC_CYLON_CTX_ARROW_MEMORY_POOL_UTILS_HPP_
