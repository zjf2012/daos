//
// (C) Copyright 2019-2020 Intel Corporation.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// GOVERNMENT LICENSE RIGHTS-OPEN SOURCE SOFTWARE
// The Government's rights to use, modify, reproduce, release, perform, display,
// or disclose this software are subject to the terms of the Apache License as
// provided in Contract No. 8F-30005.
// Any reproduction of computer software, computer software documentation, or
// portions thereof marked with this legend must also reproduce the markings.
//

package server

import (
	"github.com/pkg/errors"

	"github.com/daos-stack/daos/src/control/logging"
	"github.com/daos-stack/daos/src/control/server/ioserver"
	"github.com/daos-stack/daos/src/control/server/storage"
	"github.com/daos-stack/daos/src/control/server/storage/bdev"
	"github.com/daos-stack/daos/src/control/server/storage/scm"
)

// StorageControlService encapsulates the storage part of the control service
type StorageControlService struct {
	log             logging.Logger
	bdev            *bdev.Provider
	scm             *scm.Provider
	instanceStorage []ioserver.StorageConfig
}

// DefaultStorageControlService returns a initialized *StorageControlService
// with default behaviour
func DefaultStorageControlService(log logging.Logger, cfg *Configuration) (*StorageControlService, error) {
	return NewStorageControlService(log,
		bdev.DefaultProvider(log),
		scm.DefaultProvider(log), cfg.Servers), nil
}

// NewStorageControlService returns an initialized *StorageControlService
func NewStorageControlService(log logging.Logger, bdev *bdev.Provider, scm *scm.Provider, srvCfgs []*ioserver.Config) *StorageControlService {
	instanceStorage := []ioserver.StorageConfig{}
	for _, srvCfg := range srvCfgs {
		instanceStorage = append(instanceStorage, srvCfg.Storage)
	}

	return &StorageControlService{
		log:             log,
		bdev:            bdev,
		scm:             scm,
		instanceStorage: instanceStorage,
	}
}

// canAccessBdevs evaluates if any specified Bdevs are not accessible.
func (c *StorageControlService) canAccessBdevs(sr *bdev.ScanResponse) (missing []string, ok bool) {
	getController := func(pciAddr string) *storage.NvmeController {
		for _, s := range sr.Controllers {
			c.log.Debugf("***s.PciAddr = %s, pciAddr = %s\n", s.PciAddr, pciAddr)
			// TODO Need solution for VMD NvmeController not being
			// found (s.PciAddr = NVMe dev, pciAddr = VMD addr in
			// config)
			if s.PciAddr == pciAddr {
				return s
			}
		}
		return nil
	}

	for _, storageCfg := range c.instanceStorage {
		c.log.Debugf("***GetNvmeDevs()\n")
		for _, pciAddr := range storageCfg.Bdev.GetNvmeDevs() {
			c.log.Debugf("***%s\n", pciAddr)
			if getController(pciAddr) == nil {
				c.log.Debugf("***missing()\n")
				missing = append(missing, pciAddr)
			}
		}
	}

	return missing, len(missing) == 0
}

// Setup delegates to Storage implementation's Setup methods.
func (c *StorageControlService) Setup() error {
	c.log.Debugf("***calling Scan()\n")
	sr, err := c.bdev.Scan(bdev.ScanRequest{})
	if err != nil {
		c.log.Debugf("%s\n", errors.Wrap(err, "Warning, NVMe Scan"))
	} else {
		c.log.Debugf("***canAccessBdevs()\n")

		// fail if config specified nvme devices are inaccessible
		missing, ok := c.canAccessBdevs(sr)
		if !ok {
			return FaultBdevNotFound(missing)
		}
	}

	if _, err := c.scm.Scan(scm.ScanRequest{}); err != nil {
		c.log.Debugf("%s\n", errors.Wrap(err, "Warning, SCM Scan"))
	}

	return nil
}

// NvmePrepare preps locally attached SSDs and returns error.
//
// Suitable for commands invoked directly on server, not over gRPC.
func (c *StorageControlService) NvmePrepare(req bdev.PrepareRequest) (*bdev.PrepareResponse, error) {
	return c.bdev.Prepare(req)
}

// GetScmState performs required initialisation and returns current state
// of SCM module preparation.
func (c *StorageControlService) GetScmState() (storage.ScmState, error) {
	return c.scm.GetState()
}

// ScmPrepare preps locally attached modules and returns need to reboot message,
// list of pmem device files and error directly.
//
// Suitable for commands invoked directly on server, not over gRPC.
func (c *StorageControlService) ScmPrepare(req scm.PrepareRequest) (*scm.PrepareResponse, error) {
	// transition to the next state in SCM preparation
	return c.scm.Prepare(req)
}

// NvmeScan scans locally attached SSDs and returns list directly.
//
// Suitable for commands invoked directly on server, not over gRPC.
func (c *StorageControlService) NvmeScan() (*bdev.ScanResponse, error) {
	return c.bdev.Scan(bdev.ScanRequest{})
}

// ScmScan scans locally attached modules, namespaces and state of DCPM config.
//
// Suitable for commands invoked directly on server, not over gRPC.
func (c *StorageControlService) ScmScan() (*scm.ScanResponse, error) {
	return c.scm.Scan(scm.ScanRequest{})
}
