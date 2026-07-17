#include <gtest/gtest.h>

#include <PlayerModel.h>
#include <QJsonDocument>
#include <QJsonObject>

namespace
{
const QString g_playerJsonNoPal = "{ \
    \"player_uid\": \"uid_001\", \
    \"nickname\": \"TestPlayer\" \
}";

const QString g_playerJsonWithPal = "{ \
    \"player_uid\": \"uid_002\", \
    \"nickname\": \"RichPlayer\", \
    \"pals\": [ \
        { \"type\": \"IceFox\", \"gender\": \"Male\" }, \
        { \"type\": \"BadCatgirl\", \"gender\": \"Female\" } \
    ] \
}";

const QString g_playerJsonUpdatePal = "{ \
    \"player_uid\": \"uid_002\", \
    \"nickname\": \"RichPlayer\", \
    \"pals\": [ \
        { \"type\": \"IceFox\", \"gender\": \"Female\" } \
    ] \
}";
}  // namespace

// ==================== loadPlayerDataFromJsonObject ====================

TEST(PlayerModel, load_empty_json_returns_false)
{
    PlayerModel player;
    QJsonObject emptyObj;
    EXPECT_FALSE(player.loadPlayerDataFromJsonObject(emptyObj));
}

TEST(PlayerModel, load_missing_uid_returns_false)
{
    PlayerModel player;
    QJsonObject obj;
    obj["nickname"] = "NoUID";
    EXPECT_FALSE(player.loadPlayerDataFromJsonObject(obj));
}

TEST(PlayerModel, load_missing_nickname_returns_false)
{
    PlayerModel player;
    QJsonObject obj;
    obj["player_uid"] = "uid_xxx";
    EXPECT_FALSE(player.loadPlayerDataFromJsonObject(obj));
}

TEST(PlayerModel, load_basic_player_info)
{
    PlayerModel player;
    QJsonDocument doc = QJsonDocument::fromJson(g_playerJsonNoPal.toUtf8());
    ASSERT_TRUE(player.loadPlayerDataFromJsonObject(doc.object()));

    EXPECT_EQ(player.getPlayerUID(), QString("uid_001"));
    EXPECT_EQ(player.getPlayerNickName(), QString("TestPlayer"));
    EXPECT_FALSE(player.isPalDataLoaded());
}

TEST(PlayerModel, reloading_same_uid_is_allowed)
{
    PlayerModel player;
    QJsonDocument doc1 = QJsonDocument::fromJson(g_playerJsonNoPal.toUtf8());
    ASSERT_TRUE(player.loadPlayerDataFromJsonObject(doc1.object()));

    // 相同 uid 重新加载应成功
    EXPECT_TRUE(player.loadPlayerDataFromJsonObject(doc1.object()));
}

TEST(PlayerModel, reloading_different_uid_is_rejected)
{
    PlayerModel player;
    QJsonDocument doc1 = QJsonDocument::fromJson(g_playerJsonNoPal.toUtf8());
    ASSERT_TRUE(player.loadPlayerDataFromJsonObject(doc1.object()));

    // 不同 uid 的数据不能被加载到同一个 PlayerModel 中
    QJsonObject diffObj;
    diffObj["player_uid"] = "uid_999";
    diffObj["nickname"]   = "Stranger";
    EXPECT_FALSE(player.loadPlayerDataFromJsonObject(diffObj));
}

// NOTE: 带 pals 的加载测试需要 DisplayPalManager 预加载数据。
// 以下仅验证不带 pals 的基本场景。

// ==================== 默认状态 ====================

TEST(PlayerModel, default_state)
{
    PlayerModel player;
    EXPECT_TRUE(player.getPlayerUID().isEmpty());
    EXPECT_TRUE(player.getPlayerNickName().isEmpty());
    EXPECT_FALSE(player.isPalDataLoaded());
    EXPECT_EQ(player.getOwnedPalGendersByInterName("IceFox"), Gender::NOTSET);
    EXPECT_FALSE(player.isOwnPalByInterName("IceFox"));
}

// ==================== isApply / apply ====================

// NOTE: apply() 依赖 PlayerManager 单例，测试需要在 PlayerManager 测试中联动验证。
