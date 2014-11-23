/* SourceMap-Qt
 * (C) Copyright 2014 HicknHack Software GmbH
 *
 * The original code can be found at:
 *     https://github.com/hicknhack-software/SourceMap-Qt
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
#ifndef SOURCEMAP_FILEPOSITION_H
#define SOURCEMAP_FILEPOSITION_H

#include "SourceMap/Position.h"

#include <QString>

namespace SourceMap {

/**
 * @brief combines a (file) name and a position in that file
 *
 * an empty name marks the position invalid (think of no source is known)
 */
struct FilePosition : public Position
{
    /// constructs an invalid position
    FilePosition()
        : FilePosition(QString(), Position())
    {}

    /// constructs a valid position
    FilePosition(const QString &_name, const Position &_position)
        : Position(_position)
        , name(_name)
    {}

    inline bool isValid() const { return !name.isEmpty() && this->Position::isValid(); }

    QString name;
};

inline bool operator==(const FilePosition& l, const FilePosition& r)
{
    return (static_cast<Position>(l) == static_cast<Position>(r))
            && (l.name == r.name);
}

inline bool operator!=(const FilePosition& l, const FilePosition& r)
{
    return !(l == r);
}

} // namespace SourceMap

#endif // SOURCEMAP_FILEPOSITION_H