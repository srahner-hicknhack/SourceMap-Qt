# SourceMap-Qt
# (C) Copyright 2014 HicknHack Software GmbH
#
# The original code can be found at:
#    https://github.com/hicknhack-software/SourceMap-Qt
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#
# This is the main SourceMap-Qt project file for QMake & QtCreator
#
TEMPLATE = subdirs

SUBDIRS += src

!contains(CONFIG, NoTest) {
    text.depends = src
    SUBDIRS += test
}

OTHER_FILES += \
    CHANGES \
    LICENSE \
    NOTICE \
    README.md