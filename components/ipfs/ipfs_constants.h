/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_IPFS_IPFS_CONSTANTS_H_
#define BRAVE_COMPONENTS_IPFS_IPFS_CONSTANTS_H_

namespace ipfs {

extern const char kServerEndpoint[];
extern const char kSwarmPeersPath[];
extern const char kConfigPath[];
extern const char kArgQueryParam[];
extern const char kAddressesField[];
extern const char kShutdownPath[];
extern const char kIPFSScheme[];
extern const char kIPNSScheme[];
extern const char kDefaultIPFSGateway[];
extern const char kDefaultIPFSLocalGateway[];
extern const char kIPFSLearnMoreURL[];

enum class IPFSResolveMethodTypes {
  IPFS_ASK,
  IPFS_GATEWAY,
  IPFS_LOCAL,
  IPFS_DISABLED,
};

}  // namespace ipfs

#endif  // BRAVE_COMPONENTS_IPFS_IPFS_CONSTANTS_H_
