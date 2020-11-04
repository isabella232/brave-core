/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/ipfs/ipfs_gateway.h"

#include "base/logging.h"
#include "brave/components/ipfs/ipfs_constants.h"

namespace ipfs {

std::string ipfs_default_gateway_for_test;

void SetIPFSDefaultGatewayForTest(const std::string& url) {
  ipfs_default_gateway_for_test = url;
}

std::string GetDefaultIPFSLocalGateway() {
  return kDefaultIPFSLocalGateway;
}

std::string GetDefaultIPFSGateway() {
  if (!ipfs_default_gateway_for_test.empty()) {
    LOG(ERROR) << "=======Returning deafult gateway for test: "
               << ipfs_default_gateway_for_test;
    return ipfs_default_gateway_for_test;
  }
  LOG(ERROR) << "=======Returning normal gateway: " << kDefaultIPFSGateway;
  return kDefaultIPFSGateway;
}

}  // namespace ipfs
