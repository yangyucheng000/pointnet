# Copyright 2021 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================
"""define loss function for network"""
import mindspore.nn as nn
import mindspore.ops as ops
from mindspore import Tensor
from mindspore.common import dtype as mstype
from mindspore.nn.loss import LossBase

class CrossEntropySmooth(LossBase):
    """CrossEntropySmooth"""
    def __init__(self, smooth_factor=0., num_classes=1000):
        super(CrossEntropySmooth, self).__init__()
        self.onehot = ops.OneHot()
        self.on_value = Tensor(1.0 - smooth_factor, mstype.float32)
        self.off_value = Tensor(1.0 * smooth_factor / (num_classes - 1), mstype.float32)
        self.ce = nn.SoftmaxCrossEntropyWithLogits()
        self.mean = ops.ReduceMean(False)

    def construct(self, logit, label):
        one_hot_label = self.onehot(label, ops.Shape()(logit)[1], self.on_value, self.off_value)
        loss_logit = self.ce(logit, one_hot_label)
        loss_logit = self.mean(loss_logit, 0)
        return loss_logit
