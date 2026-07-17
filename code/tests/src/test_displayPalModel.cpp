#include <gtest/gtest.h>

#include <DisplayPalModel.h>
#include <QJsonDocument>
#include <QJsonObject>

namespace
{
const QString g_rawPalJsonData = "{ \
    \"Name\": \"Nyafia\", \
    \"LocalizedNames\": { \
        \"de\": \"Nyafia\", \
        \"en\": \"Nyafia\", \
        \"es-MX\": \"Nyafia\", \
        \"es\": \"Nyafia\", \
        \"fr\": \"Nyafia\", \
        \"id\": \"Nyafia\", \
        \"it\": \"Nyafia\", \
        \"ko\": \"멀보냥\", \
        \"pl\": \"Nyafia\", \
        \"pt-BR\": \"Nyafia\", \
        \"ru\": \"Няфия\", \
        \"th\": \"Nyafia\", \
        \"tr\": \"Nyafia\", \
        \"vi\": \"Nyafia\", \
        \"zh-Hans\": \"妮瞅莎\", \
        \"zh-Hant\": \"妮瞅莎\", \
        \"ja\": \"ニャニミテ\" \
    }, \
    \"InternalName\": \"BadCatgirl\" \
}";
}

TEST(DisplayPalModel, load)
{
    QJsonDocument doc = QJsonDocument::fromJson(g_rawPalJsonData.toUtf8());

    DisplayPalModel palModel;
    ASSERT_TRUE(palModel.loadPalModel(doc.object()));
    ASSERT_EQ(palModel.getInternalName(), QString::fromUtf8("BadCatgirl"));
    ASSERT_EQ(palModel.getLocalizedName(), QString::fromUtf8("妮瞅莎"));

    ASSERT_TRUE(palModel.getOwnedGender() & Gender::NOTSET);

    palModel.setOwnedGender(Gender::Male);
    ASSERT_TRUE(palModel.getOwnedGender() & Gender::Male);
    ASSERT_FALSE(palModel.getOwnedGender() & Gender::Female);
    ASSERT_FALSE(palModel.getOwnedGender() & Gender::NOTSET);

    palModel.setOwnedGender(Gender::Male | Gender::Female);
    ASSERT_TRUE(palModel.getOwnedGender() & Gender::Male);
    ASSERT_TRUE(palModel.getOwnedGender() & Gender::Female);
    ASSERT_FALSE(palModel.getOwnedGender() & Gender::NOTSET);
}

TEST(DisplayPalModel, owned)
{
    QJsonDocument doc = QJsonDocument::fromJson(g_rawPalJsonData.toUtf8());

    DisplayPalModel palModel;
    ASSERT_TRUE(palModel.loadPalModel(doc.object()));

    ASSERT_FALSE(palModel.isOwned());
    palModel.setOwned(true);
    ASSERT_TRUE(palModel.isOwned());
}

TEST(DisplayPalModel, gender)
{
    QJsonDocument doc = QJsonDocument::fromJson(g_rawPalJsonData.toUtf8());

    DisplayPalModel palModel;
    ASSERT_TRUE(palModel.loadPalModel(doc.object()));

    ASSERT_TRUE(palModel.getOwnedGender() & Gender::NOTSET);

    palModel.setOwnedGender(Gender::Male);
    ASSERT_TRUE(palModel.getOwnedGender() & Gender::Male);
    ASSERT_FALSE(palModel.getOwnedGender() & Gender::Female);
    ASSERT_FALSE(palModel.getOwnedGender() & Gender::NOTSET);

    palModel.setOwnedGender(Gender::Male | Gender::Female);
    ASSERT_TRUE(palModel.getOwnedGender() & Gender::Male);
    ASSERT_TRUE(palModel.getOwnedGender() & Gender::Female);
    ASSERT_FALSE(palModel.getOwnedGender() & Gender::NOTSET);
}
