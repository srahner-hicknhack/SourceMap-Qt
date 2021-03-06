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
#include "SourceMap/Extension/Caller.h"

#include "SourceMap/Entry.h"
#include "SourceMap/Mapping.h"
#include "SourceMap/RevisionThree.h"

#include <QObject>
#include <QString>
#include <QtTest>

#include <tuple>

class TestExtensionCaller : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void value();
    void entry();
    void mapping();
    void revisionThree();
};

namespace {

using Caller = SourceMap::Caller;
using CallerList = SourceMap::CallerList;
using CallerStack = SourceMap::CallerStack;
using CallerIndex = SourceMap::CallerIndex;
using Mapping = SourceMap::Mapping< SourceMap::Extension::Caller >;
using Entry = Mapping::Entry;
using Data = Mapping::Data;
using EntryList = Mapping::EntryList;

#define SOURCE_ONE "sourceOne"
#define SOURCE_TWO "sourceTwo"
#define GENERATED "generated"
#define SYMBOL "symbol"

EntryList buildEntries()
{
    return {{
            Entry{{1,1}},
            Entry{{1,10},{SOURCE_ONE,{1,1}}, std::make_tuple(CallerIndex{0})},
            Entry{{1,15}},
            Entry{{2,10},{SOURCE_TWO,{1,1}}, SYMBOL, std::make_tuple(CallerIndex{2})},
            Entry{{2,1}},
            Entry{{2,20}},
        }};
}

CallerList buildCallerList()
{
    return {{
            Caller{{SOURCE_TWO,{20,1}}, CallerIndex{-1}},
            Caller{{SOURCE_TWO,{40,1}}, CallerIndex{-1}},
            Caller{{SOURCE_TWO,{40,10}}, CallerIndex{1}},
        }};
}

} // namespace

void TestExtensionCaller::value()
{
    auto p1 = CallerIndex{};
    QCOMPARE(p1.value, InvalidCallerIndex);

    auto p2 = CallerIndex{99};
    QCOMPARE(p2.value, 99);

    auto c1 = Caller{};
    QVERIFY(!c1.isValid());

    auto c2 = Caller{{SOURCE_ONE, {3,4}}};
    QVERIFY(c2.isValid());
    QCOMPARE(c2.original, (SourceMap::FilePosition{SOURCE_ONE, {3,4}}));
    QCOMPARE(c2.parentIndex.value, InvalidCallerIndex);

    auto c3 = Caller{{SOURCE_ONE, {3,4}}, CallerIndex{2}};
    QCOMPARE(c3.parentIndex.value, 2);
}

#define TEST_STR1 "hello"

void TestExtensionCaller::entry()
{
    using CallerExt = SourceMap::Extension::Caller;
    using SourceMap::get;

    auto p2 = Entry{{3,4}};
    QCOMPARE(get<CallerExt>(p2).value, InvalidCallerIndex);

    auto p4 = Entry{{3,4}, {TEST_STR1, {6,4}}, TEST_STR1, std::make_tuple(CallerIndex{32})};
    QCOMPARE(get<CallerExt>(p4).value, 32);
}

void TestExtensionCaller::mapping()
{
    using FilePosition = SourceMap::FilePosition;

    auto map = Mapping{{buildEntries(), std::make_tuple(buildCallerList())}};
    auto e1 = map.findEntryByGenerated({1,5});
    QVERIFY(e1 != nullptr);

    auto p1 = SourceMap::buildCallerStack(map.data(), e1);
    QVERIFY(p1.empty());

    auto e2 = map.findEntryByGenerated({2,13});
    QVERIFY(e2 != nullptr);

    auto p2 = SourceMap::buildCallerStack(map.data(), e2);
    QCOMPARE(p2.size(), 2u);
    QCOMPARE(p2[0], FilePosition(SOURCE_TWO, {40,10}));
}

void TestExtensionCaller::revisionThree()
{
    using FilePosition = SourceMap::FilePosition;
    using RevisionThree = SourceMap::RevisionThree;

    auto m = Mapping{{buildEntries(), std::make_tuple(buildCallerList())}};

    RevisionThree r3;
    QCOMPARE(r3.version(), 3);

    r3.storeFile(GENERATED);
    QCOMPARE(r3.file(), QString(GENERATED));

    r3.encodeMappings(m);
    QCOMPARE(r3.sources().size(), 2);
    QCOMPARE(r3.names().size(), 1);

    auto bytes = r3.toJson();

    QJsonParseError error;
    auto rr = RevisionThree::fromJson(bytes, &error);
    QVERIFY(error.error == QJsonParseError::NoError);
    QCOMPARE(rr.names(), r3.names());

    auto rm = Mapping{ rr.decodedMappings< Data >() };
    QCOMPARE(rm.data().entries.size(), m.data().entries.size());
    QCOMPARE(rm.originalNames(), m.originalNames());

    const auto* e2 = rm.findEntryByGenerated({2,13});
    QVERIFY(e2 != nullptr);

    auto p2 = SourceMap::buildCallerStack(rm, e2);
    QCOMPARE(p2.size(), 2u);
    QCOMPARE(p2[0], FilePosition(SOURCE_TWO, {40,10}));
}

QTEST_GUILESS_MAIN(TestExtensionCaller)

#include "TestExtensionCaller.moc"

