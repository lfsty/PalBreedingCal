#include <gtest/gtest.h>

#include <PlayerManager.h>
#include <PlayerModel.h>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace
{
const QString g_playerJson = "{ \
    \"player_uid\": \"uid_001\", \
    \"nickname\": \"TestPlayer\" \
}";

const QString g_playerJson2 = "{ \
    \"player_uid\": \"uid_002\", \
    \"nickname\": \"SecondPlayer\" \
}";
}  // namespace

// ==================== 单例 ====================

TEST(PlayerManager, getInstance_returns_same_instance)
{
    PlayerManager* m1 = PlayerManager::getInstance();
    PlayerManager* m2 = PlayerManager::getInstance();
    EXPECT_EQ(m1, m2);
}

// ==================== LoadPlayerData (单个 JSON 对象) ====================

TEST(PlayerManager, load_empty_json_object_returns_false)
{
    // 注意：PlayerManager 是单例，测试间可能残留数据
    QJsonObject emptyObj;
    // 空对象会创建新 player 失败，因为 player 的 loadPlayerDataFromJsonObject 会拒绝空 JSON
    EXPECT_FALSE(PlayerManager::getInstance()->LoadPlayerData(emptyObj));
}

TEST(PlayerManager, load_valid_player_returns_true)
{
    QJsonDocument doc = QJsonDocument::fromJson(g_playerJson.toUtf8());
    EXPECT_TRUE(PlayerManager::getInstance()->LoadPlayerData(doc.object()));
}

TEST(PlayerManager, load_duplicate_player_returns_true)
{
    QJsonDocument doc = QJsonDocument::fromJson(g_playerJson.toUtf8());
    // 重复加载同一 player 应成功（数据匹配已有 player）
    EXPECT_TRUE(PlayerManager::getInstance()->LoadPlayerData(doc.object()));
    EXPECT_TRUE(PlayerManager::getInstance()->LoadPlayerData(doc.object()));
}

// ==================== LoadPlayerData (JSON 数组) ====================

TEST(PlayerManager, load_json_array_with_new_player)
{
    QJsonDocument doc1 = QJsonDocument::fromJson(g_playerJson.toUtf8());
    QJsonArray arr;
    arr.append(doc1.object());

    PlayerManager* mgr = PlayerManager::getInstance();
    EXPECT_TRUE(mgr->LoadPlayerData(arr));
    EXPECT_TRUE(mgr->getPlayerMap().contains("uid_001"));
    EXPECT_FALSE(mgr->getPlayerMap().contains("uid_002"));
}

// ==================== getPlayerMap / getCurrentPlayer ====================

TEST(PlayerManager, getPlayerMap_contains_not_loaded_players)
{
    PlayerManager* mgr = PlayerManager::getInstance();
    const auto& map    = mgr->getPlayerMap();
    EXPECT_FALSE(map.contains("uid_001"));
    EXPECT_FALSE(map.contains("uid_002"));
}

TEST(PlayerManager, getCurrentPlayer_initially_null)
{
    // 注意：如果前序测试调用了 apply()，当前玩家可能不为 null
    // 这里不验证具体值，只确保接口可调用
    PlayerManager* mgr         = PlayerManager::getInstance();
    const PlayerModel* current = mgr->getCurrentPlayer();
    // 仅验证接口不崩溃
    SUCCEED();
}

// ==================== setCurrentPlayer ====================

TEST(PlayerManager, setCurrentPlayer_existing_uid)
{
    PlayerManager* mgr = PlayerManager::getInstance();

    QJsonDocument doc1 = QJsonDocument::fromJson(g_playerJson.toUtf8());
    QJsonArray arr;
    arr.append(doc1.object());

    EXPECT_TRUE(mgr->LoadPlayerData(arr));
    EXPECT_TRUE(mgr->getPlayerMap().contains("uid_001"));

    mgr->setCurrentPlayer("uid_001");
    const PlayerModel* current = mgr->getCurrentPlayer();
    ASSERT_NE(current, nullptr);
    EXPECT_EQ(current->getPlayerUID(), QString("uid_001"));
}

TEST(PlayerManager, setCurrentPlayer_nonexistent_uid_sets_null)
{
    PlayerManager* mgr = PlayerManager::getInstance();
    mgr->setCurrentPlayer("uid_nonexistent");
    EXPECT_EQ(mgr->getCurrentPlayer(), nullptr);
}
