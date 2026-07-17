#include <gtest/gtest.h>

#include <BreedingModel.h>
#include <DisplayPalModel.h>
#include <QJsonDocument>
#include <QJsonObject>

namespace
{
const QString g_palJson1 = "{ \
    \"Name\": \"Nyafia\", \
    \"LocalizedNames\": { \
        \"zh-Hans\": \"妮瞅莎\" \
    }, \
    \"InternalName\": \"BadCatgirl\" \
}";

const QString g_palJson2 = "{ \
    \"Name\": \"Kitsun\", \
    \"LocalizedNames\": { \
        \"zh-Hans\": \"火狐狸\" \
    }, \
    \"InternalName\": \"IceFox\" \
}";

const QString g_palJson3 = "{ \
    \"Name\": \"Deer\", \
    \"LocalizedNames\": { \
        \"zh-Hans\": \"冰鹿\" \
    }, \
    \"InternalName\": \"IceDeer\" \
}";

// 辅助函数：从 JSON 创建 DisplayPalModel
DisplayPalModel* makePal(const QString& json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    auto* pal         = new DisplayPalModel();
    pal->loadPalModel(doc.object());
    return pal;
}
}  // namespace

// ==================== copy ====================

TEST(BreedingModel, copy_without_swap)
{
    DisplayPalModel* p1 = makePal(g_palJson1);
    DisplayPalModel* p2 = makePal(g_palJson2);
    DisplayPalModel* c  = makePal(g_palJson3);

    BreedingModel original;
    original.parent1 = p1;
    original.parent2 = p2;
    original.child   = c;

    BreedingModel copied = original.copy(false);

    EXPECT_EQ(copied.parent1, p1);
    EXPECT_EQ(copied.parent2, p2);
    EXPECT_EQ(copied.child, c);

    delete p1;
    delete p2;
    delete c;
}

TEST(BreedingModel, copy_with_swap)
{
    DisplayPalModel* p1 = makePal(g_palJson1);
    DisplayPalModel* p2 = makePal(g_palJson2);
    DisplayPalModel* c  = makePal(g_palJson3);

    BreedingModel original;
    original.parent1 = p1;
    original.parent2 = p2;
    original.child   = c;

    BreedingModel swapped = original.copy(true);

    EXPECT_EQ(swapped.parent1, p2);
    EXPECT_EQ(swapped.parent2, p1);
    EXPECT_EQ(swapped.child, c);

    delete p1;
    delete p2;
    delete c;
}

TEST(BreedingModel, copy_does_not_affect_original)
{
    DisplayPalModel* p1 = makePal(g_palJson1);
    DisplayPalModel* p2 = makePal(g_palJson2);
    DisplayPalModel* c  = makePal(g_palJson3);

    BreedingModel original;
    original.parent1 = p1;
    original.parent2 = p2;
    original.child   = c;

    BreedingModel copied = original.copy(true);

    // 原对象不应被修改
    EXPECT_EQ(original.parent1, p1);
    EXPECT_EQ(original.parent2, p2);
    EXPECT_EQ(original.child, c);

    delete p1;
    delete p2;
    delete c;
}

// ==================== 默认构造 ====================

TEST(BreedingModel, default_construction)
{
    BreedingModel bm;
    EXPECT_EQ(bm.parent1, nullptr);
    EXPECT_EQ(bm.parent2, nullptr);
    EXPECT_EQ(bm.child, nullptr);
}
