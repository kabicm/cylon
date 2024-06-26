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

#include <memory>
#include <cstring>
#include <string>
#include <iostream>

#include <cylon/net/cylon_request.hpp>
#include <cylon/util/builtins.hpp>

cylon::CylonRequest::CylonRequest(int tgt) {
  target = tgt;
}

cylon::CylonRequest::CylonRequest(int tgt, const void *buf, int len) {
  target = tgt;
  buffer = buf;
  length = len;
}

cylon::CylonRequest::CylonRequest(int tgt, const void *buf, int len, int *head, int hLength) {
  target = tgt;
  buffer = buf;
  length = len;
  memcpy(&header[0], head, hLength * sizeof(int));
  headerLength = hLength;
}

cylon::CylonRequest::~CylonRequest() {
  buffer = nullptr;
}

void cylon::CylonRequest::to_string(std::string dataType, int bufDepth) {
  std::cout << "Target: " << target << std::endl;
  std::cout << "Length: " << length << std::endl;
  std::cout << "Header Length: " << headerLength << std::endl;
  std::cout << "Buffer: " << std::endl;
  cylon::util::printArray(buffer, length, dataType, bufDepth);
  std::cout << "Header: " << std::endl;
  for (int i = 0; i < headerLength; ++i) {
    std::cout << header[i] << " ";
  }
  std::cout << std::endl;
}
