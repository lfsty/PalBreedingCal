#include <gtest/gtest.h>

#include <PlayerPalModel.h>
#include <QJsonDocument>
#include <QJsonObject>

namespace
{
const QString g_validPalJson = "{ \
    \"type\": \"IceFox\", \
    \"gender\": \"Male\" \
}";

const QString g_bossPalJson = "{ \
    \"type\": \"BOSS_IceFox\", \
    \"gender\": \"Female\" \
}";
}  // namespace

// ==================== loadPalModel: 边界条件 ====================

TEST(PlayerPalModel, load_empty_json_returns_false)
{
    PlayerPalModel model;
    QJsonObject emptyObj;
    EXPECT_FALSE(model.loadPalModel(emptyObj));
}

TEST(PlayerPalModel, load_missing_type_key_returns_false)
{
    PlayerPalModel model;
    QJsonObject obj;
    obj["gender"] = "Male";
    EXPECT_FALSE(model.loadPalModel(obj));
}

TEST(PlayerPalModel, load_missing_gender_key_returns_false)
{
    PlayerPalModel model;
    QJsonObject obj;
    obj["type"] = "IceFox";
    EXPECT_FALSE(model.loadPalModel(obj));
}

// NOTE: 以下测试需要 DisplayPalManager 预加载 Pal 数据后才能完整验证。
// 当前仅验证边界条件（空 JSON / 缺少必要字段）。
// 完整流程测试（包含 type 解析、Boss 前缀处理、DisplayPalManager 查找）
// 需要先初始化 DisplayPalManager 并加载 db.json。

// ==================== 默认构造 ====================

TEST(PlayerPalModel, default_gender_is_notset)
{
    PlayerPalModel model;
    EXPECT_EQ(model.getGender(), Gender::NOTSET);
}

TEST(PlayerPalModel, default_internal_name_is_empty)
{
    PlayerPalModel model;
    EXPECT_TRUE(model.getInternalName().isEmpty());
}
