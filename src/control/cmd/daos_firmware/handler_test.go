//
// (C) Copyright 2020 Intel Corporation.
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

package main

import (
	"encoding/json"
	"errors"
	"testing"

	"github.com/daos-stack/daos/src/control/common"
	"github.com/daos-stack/daos/src/control/fault"
	"github.com/daos-stack/daos/src/control/logging"
	"github.com/daos-stack/daos/src/control/pbin"
	"github.com/daos-stack/daos/src/control/server/storage"
	"github.com/daos-stack/daos/src/control/server/storage/scm"
	"github.com/google/go-cmp/cmp"
)

func expectPayload(t *testing.T, resp *pbin.Response, payload interface{}, expPayload interface{}) {
	t.Helper()

	err := json.Unmarshal(resp.Payload, payload)
	if err != nil {
		t.Fatalf("couldn't unmarshal response payload")
	}

	if diff := cmp.Diff(expPayload, payload); diff != "" {
		t.Errorf("got wrong payload (-want, +got)\n%s\n", diff)
	}
}

// TODO KJ: update when SCM logic in place
func TestDaosFirmware_ScmQueryHandler(t *testing.T) {
	scmQueryReqPayload, err := json.Marshal(scm.FirmwareQueryRequest{
		ForwardableRequest: pbin.ForwardableRequest{Forwarded: true},
	})
	if err != nil {
		t.Fatal(err)
	}

	for name, tc := range map[string]struct {
		req        *pbin.Request
		smbc       *scm.MockBackendConfig
		smsc       *scm.MockSysConfig
		expPayload *scm.FirmwareQueryResponse
		expErr     *fault.Fault
	}{
		"nil request": {
			expErr: pbin.PrivilegedHelperRequestFailed("nil request"),
		},
		"ScmQueryFirmware nil payload": {
			req: &pbin.Request{
				Method: "ScmQueryFirmware",
			},
			expErr: pbin.PrivilegedHelperRequestFailed("unexpected end of JSON input"),
		},
		"ScmQueryFirmware success": {
			req: &pbin.Request{
				Method:  "ScmQueryFirmware",
				Payload: scmQueryReqPayload,
			},
			expPayload: &scm.FirmwareQueryResponse{},
		},
		"ScmQueryFirmware failure": {
			req: &pbin.Request{
				Method:  "ScmQueryFirmware",
				Payload: scmQueryReqPayload,
			},
			smbc: &scm.MockBackendConfig{
				DiscoverRes: storage.ScmModules{
					&storage.ScmModule{UID: "DeviceUID"},
				},
				GetFirmwareStatusErr: errors.New("getFirmwareStatus failed"),
			},
			expErr: pbin.PrivilegedHelperRequestFailed("getFirmwareStatus failed"),
		},
	} {
		t.Run(name, func(t *testing.T) {
			log, buf := logging.NewTestLogger(name)
			defer common.ShowBufferOnFailure(t, buf)

			sp := scm.NewMockProvider(log, tc.smbc, tc.smsc)
			handler := &scmQueryHandler{scmHandler: scmHandler{scmProvider: sp}}

			resp := handler.Handle(log, tc.req)

			if diff := cmp.Diff(tc.expErr, resp.Error); diff != "" {
				t.Errorf("got wrong fault (-want, +got)\n%s\n", diff)
			}
			if tc.expPayload == nil {
				tc.expPayload = &scm.FirmwareQueryResponse{}
			}
			expectPayload(t, resp, &scm.FirmwareQueryResponse{}, tc.expPayload)
		})
	}
}

func TestDaosFirmware_ScmUpdateHandler(t *testing.T) {
	scmUpdateReqPayload, err := json.Marshal(scm.FirmwareUpdateRequest{
		ForwardableRequest: pbin.ForwardableRequest{Forwarded: true},
	})
	if err != nil {
		t.Fatal(err)
	}

	for name, tc := range map[string]struct {
		req        *pbin.Request
		smbc       *scm.MockBackendConfig
		smsc       *scm.MockSysConfig
		expPayload *scm.FirmwareUpdateResponse
		expErr     *fault.Fault
	}{
		"nil request": {
			expErr: pbin.PrivilegedHelperRequestFailed("nil request"),
		},
		"ScmQueryFirmware nil payload": {
			req: &pbin.Request{
				Method: "ScmUpdateFirmware",
			},
			expErr: pbin.PrivilegedHelperRequestFailed("unexpected end of JSON input"),
		},
		"ScmQueryFirmware success": {
			req: &pbin.Request{
				Method:  "ScmUpdateFirmware",
				Payload: scmUpdateReqPayload,
			},
			expPayload: &scm.FirmwareUpdateResponse{
				Results: map[string]error{
					"Device1": nil,
				},
			},
		},
		"ScmQueryFirmware failure": {
			req: &pbin.Request{
				Method:  "ScmUpdateFirmware",
				Payload: scmUpdateReqPayload,
			},
			smbc: &scm.MockBackendConfig{
				UpdateFirmwareErr: errors.New("UpdateFirmware failed"),
			},
			expErr: pbin.PrivilegedHelperRequestFailed("UpdateFirmware failed"),
		},
	} {
		t.Run(name, func(t *testing.T) {
			log, buf := logging.NewTestLogger(name)
			defer common.ShowBufferOnFailure(t, buf)

			sp := scm.NewMockProvider(log, tc.smbc, tc.smsc)
			handler := &scmQueryHandler{scmHandler: scmHandler{scmProvider: sp}}

			resp := handler.Handle(log, tc.req)

			if diff := cmp.Diff(tc.expErr, resp.Error); diff != "" {
				t.Errorf("got wrong fault (-want, +got)\n%s\n", diff)
			}
			if tc.expPayload == nil {
				tc.expPayload = &scm.FirmwareUpdateResponse{}
			}
			expectPayload(t, resp, &scm.FirmwareUpdateResponse{}, tc.expPayload)
		})
	}
}
