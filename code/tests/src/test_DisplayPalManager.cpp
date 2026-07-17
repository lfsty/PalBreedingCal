#include <gtest/gtest.h>

#include <DisplayPalManager.h>
#include <DisplayPalModel.h>
#include <PlayerModel.h>
#include <QJsonDocument>
#include <QJsonObject>

namespace
{
const QString g_testPalJson1 = "{ \
    \"Name\": \"Fox\", \
    \"LocalizedNames\": { \"zh-Hans\": \"火狐\" }, \
    \"InternalName\": \"TestFox\" \
}";

const QString g_testPalJson2 = "{ \
    \"Name\": \"Cat\", \
    \"LocalizedNames\": { \"zh-Hans\": \"黑猫\" }, \
    \"InternalName\": \"TestCat\" \
}";

const QString g_testPalJson3 = "{ \
    \"Name\": \"Deer\", \
    \"LocalizedNames\": { \"zh-Hans\": \"冰鹿\" }, \
    \"InternalName\": \"TestDeer\" \
}";
}  // namespace

// ==================== 单例 ====================

TEST(DisplayPalManager, getInstance_returns_same_instance)
{
    DisplayPalManager* m1 = DisplayPalManager::getInstance();
    DisplayPalManager* m2 = DisplayPalManager::getInstance();
    EXPECT_EQ(m1, m2);
}

// ==================== getDisplayPalModel: 未加载时返回 nullptr ====================

TEST(DisplayPalManager, getDisplayPalModel_returns_null_when_not_loaded)
{
    // 未加载任何数据时，查找任意名称都应返回 nullptr
    const DisplayPalModel* result = DisplayPalManager::getInstance()->getDisplayPalModel("NonExistent");
    EXPECT_EQ(result, nullptr);
}

// ==================== getBreedingListByFilter: 空参数 ====================

TEST(DisplayPalManager, getBreedingListByFilter_all_empty_returns_empty)
{
    auto result = DisplayPalManager::getInstance()->getBreedingListByFilter("", "", "");
    EXPECT_TRUE(result.empty());
}

// ==================== updataOwnedPal: nullptr 安全 ====================

TEST(DisplayPalManager, updataOwnedPal_nullptr_does_not_crash)
{
    // 传入 nullptr 不应崩溃
    EXPECT_NO_FATAL_FAILURE(DisplayPalManager::getInstance()->updataOwnedPal(nullptr));
}

// NOTE:
// 以下功能需要 DisplayPalManager 先通过 requestLoadDB() 加载 db.json 和 breeding.json 后才能完整测试：
//   - getBreedingList()
//   - getPalLocalNameList()
//   - getBreedingListByFilter() 的非空参数场景
//   - updataOwnedPal() 的实际更新逻辑
// 由于 requestLoadDB() 内部使用 QtConcurrent::run() 异步加载，
// 需要配合 QSignalSpy 或 QEventLoop 等待 dataLoaded 信号后再进行验证。
// 建议编写集成测试或在 setUp() 中手动构造内部数据结构。
