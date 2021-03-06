/*
 * Copyright 2021 Huawei Technologies Co., Ltd All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SSDGhost.h"
#include "MxBase/Log/Log.h"

namespace {
    const uint32_t CLASS_NUM = 81;
}

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        LogWarn << "Please input image path, such as './ssd_ghost ssd_ghostnet.om test.jpg'.";
        return APP_ERR_OK;
    }

    InitParam initParam = {};
    initParam.deviceId = 0;
    initParam.classNum = CLASS_NUM;
    initParam.labelPath = "../models/coco.names";

    initParam.iou_thresh = 0.6;
    initParam.score_thresh = 0.6;
    initParam.checkTensor = true;

    initParam.modelPath = argv[1];
    auto ssdGhost = std::make_shared<SSDGhost>();
    APP_ERROR ret = ssdGhost->Init(initParam);
    if (ret != APP_ERR_OK) {
        LogError << "SsdGhost init failed, ret=" << ret << ".";
        return ret;
    }

    std::string imgPath = argv[2];
    ret = ssdGhost->Process(imgPath);
    if (ret != APP_ERR_OK) {
        LogError << "SsdGhost process failed, ret=" << ret << ".";
        ssdGhost->DeInit();
        return ret;
    }
    ssdGhost->DeInit();
    return APP_ERR_OK;
}
