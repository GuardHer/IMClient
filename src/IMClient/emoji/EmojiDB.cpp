
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <regex>
// #include <boost/algorithm/string.hpp>

#include "EmojiDB.hpp"
#include "simple-json.hpp"
#include "tinyutf8.hpp"


Emoji::Emoji(const std::string& str, const std::string& name,
             std::unordered_set<std::string>&& keywords, const bool hasSkinToneSupport,
             const EmojiVersion version) :
    _str {str},
    _name {name},
    _keywords {std::move(keywords)},
    _hasSkinToneSupport {hasSkinToneSupport},
    _version {version}
{
}

const std::string& Emoji::lcName() const
{
    if (_lcName.empty()) {
        _lcName = _name;
        std::transform(_lcName.begin(), _lcName.end(), _lcName.begin(), ::tolower);

    }

    return _lcName;
}

std::string Emoji::strWithSkinTone(const SkinTone skinTone) const
{
    assert(_hasSkinToneSupport);

    const auto codepoints = this->codepointsWithSkinTone(skinTone);

    return utf8_string {std::begin(codepoints), std::end(codepoints)}.c_str();  // UTF-8 字符串
}

Emoji::Codepoints Emoji::codepointsWithSkinTone(const SkinTone skinTone) const
{
    assert(_hasSkinToneSupport);

    auto codepoints = this->codepoints();  
    Codepoint skinToneCodepoint = 0;

    switch (skinTone) {
    case SkinTone::LIGHT:
        skinToneCodepoint = 0x1f3fb;
        break;

    case SkinTone::MEDIUM_LIGHT:
        skinToneCodepoint = 0x1f3fc;
        break;

    case SkinTone::MEDIUM:
        skinToneCodepoint = 0x1f3fd;
        break;

    case SkinTone::MEDIUM_DARK:
        skinToneCodepoint = 0x1f3fe;
        break;

    case SkinTone::DARK:
        skinToneCodepoint = 0x1f3ff;
        break;

    default:
        std::abort();
    }

    codepoints.insert(std::begin(codepoints) + 1, skinToneCodepoint);
    return codepoints;
}

/**
 * @function codepoints
 * @brief 获取代码点
 * @return : 代码点
 */
Emoji::Codepoints Emoji::codepoints() const
{
    Codepoints codepoints;  // 代码点 
    const utf8_string utf8Str {_str};  // UTF-8 字符串

    std::copy(std::begin(utf8Str), std::end(utf8Str), std::back_inserter(codepoints)); // 复制 UTF-8 字符串到代码点
    return codepoints;
}

EmojiCat::EmojiCat(const std::string& id, const std::string& name,
                   std::vector<const Emoji *>&& emojis) :
    _id {id},
    _name {name},
    _emojis {std::move(emojis)}
{
}

EmojiCat::EmojiCat(const std::string& id, const std::string& name) :
    _id {id},
    _name {name}
{
}

const std::string& EmojiCat::lcName() const
{
    if (_lcName.empty()) {
        _lcName = _name;
        std::transform(_lcName.begin(), _lcName.end(), _lcName.begin(), ::tolower);
    }

    return _lcName;
}

//EmojiDb::EmojiDb(const std::string& dir, const EmojiSize emojiSize) :
//    _emojiSize {emojiSize},
//    _emojisPngPath {dir + '/' + "emojis-" + std::to_string(this->emojiSizeInt()) + ".png"}
//{
//    this->_createEmojis(dir);  // 创建表情符号
//    this->_createCats(dir);  // 创建表情符号分类
//    this->_createEmojiPngLocations(dir);  // 创建表情符号 PNG 位置
//    _initialized = true;
//}

EmojiDb::EmojiDb() :
    _emojiSize{ EmojiSize::SIZE_32 },
    _emojisPngPath{ "" }
{
}


void EmojiDb::init(const std::string& dir, const EmojiSize emojiSize)
{
    if (!_initialized) {
		_emojiSize = emojiSize;
		_emojisPngPath = dir + '/' + "emojis-" + std::to_string(this->emojiSizeInt()) + ".png";
		this->_createEmojis(dir);
		this->_createCats(dir);
		this->_createEmojiPngLocations(dir);
		_initialized = true;
	}
}

json::JSON EmojiDb::_loadJson(const std::string& dir, const std::string& file)
{
    std::ifstream f {dir + '/' + file};
    std::string str {std::istreambuf_iterator<char> {f}, std::istreambuf_iterator<char> {}};
    return json::JSON::Load(str);
}


EmojiVersion versionFromJson(const json::JSON& versionJson)
{
    const auto str = versionJson.ToString();

    if (str == "0.6") {
        return EmojiVersion::V_0_6;
    } else if (str == "0.7") {
        return EmojiVersion::V_0_7;
    } else if (str == "1.0") {
        return EmojiVersion::V_1_0;
    } else if (str == "2.0") {
        return EmojiVersion::V_2_0;
    } else if (str == "3.0") {
        return EmojiVersion::V_3_0;
    } else if (str == "4.0") {
        return EmojiVersion::V_4_0;
    } else if (str == "5.0") {
        return EmojiVersion::V_5_0;
    } else if (str == "11.0") {
        return EmojiVersion::V_11_0;
    } else if (str == "12.0") {
        return EmojiVersion::V_12_0;
    } else if (str == "12.1") {
        return EmojiVersion::V_12_1;
    } else if (str == "13.0") {
        return EmojiVersion::V_13_0;
    } else if (str == "13.1") {
        return EmojiVersion::V_13_1;
    } else {
        assert(str == "14.0");
        return EmojiVersion::V_14_0;
    }
}


void EmojiDb::_createEmojis(const std::string& dir)
{
    const auto emojisJson = this->_loadJson(dir, "emojis.json");

    for (const auto& keyValPair : emojisJson.ObjectRange()) {
        const auto& emojiStr = keyValPair.first;
        const auto& valJson = keyValPair.second;
        const auto& nameJson = valJson.at("name");
        const auto& hasSkinToneSupport = valJson.at("has-skin-tone-support").ToBool();
        const auto& keywordsJson = valJson.at("keywords");
        const auto& versionJson = valJson.at("version");
        std::unordered_set<std::string> keywords;

        for (const auto& kw : keywordsJson.ArrayRange()) {
            keywords.insert(kw.ToString());
        }

        auto emoji = std::make_unique<const Emoji>(emojiStr, nameJson.ToString(),
                                                   std::move(keywords), hasSkinToneSupport,
                                                   versionFromJson(versionJson));

        for (const auto& keyword : emoji->keywords()) {
            _keywords.insert(keyword);

            auto it = _keywordEmojis.find(keyword);

            if (it == std::end(_keywordEmojis)) {
                it = _keywordEmojis.insert(std::make_pair(keyword,
                                                          decltype(_keywordEmojis)::mapped_type {})).first;
            }

            it->second.insert(emoji.get());
        }

        _emojis[emojiStr] = std::move(emoji);
    }
}

void EmojiDb::_createCats(const std::string& dir)
{
    // first, special category: recent emojis
    _cats.push_back(std::make_unique<EmojiCat>("recent", "Recent"));
    _recentEmojisCat = _cats.back().get();

    const auto catsJson = this->_loadJson(dir, "cats.json");

    for (const auto& catJson : catsJson.ArrayRange()) {
        const auto& idJson = catJson.at("id");
        const auto& nameJson = catJson.at("name");
        const auto& emojisJson = catJson.at("emojis");
        std::vector<const Emoji *> emojis;

        for (const auto& emojiJson : emojisJson.ArrayRange()) {
            const auto emojiStr = emojiJson.ToString();

            emojis.push_back(_emojis[emojiStr].get());
        }

        auto cat = std::make_unique<EmojiCat>(idJson.ToString(), nameJson.ToString(),
                                              std::move(emojis));
       
        auto catName = cat->name();
        auto iconPath = dir + "/catsIcon-48/" + std::move(catName) + ".png";
        cat->setIcon(std::move(iconPath));
        _cats.push_back(std::move(cat));
    }
}

void EmojiDb::_createEmojiPngLocations(const std::string& dir)
{
    const std::string fileName {
        "emojis-png-locations-" + std::to_string(this->emojiSizeInt()) + ".json"
    };
    const auto pngLocationsJson = this->_loadJson(dir, fileName);

    for (const auto& keyValPair : pngLocationsJson.ObjectRange()) {
        const auto& emojiStr = keyValPair.first;
        const auto& valJson = keyValPair.second;

        _emojiPngLocations[_emojis[emojiStr].get()] = {
            static_cast<unsigned int>(valJson.at(0).ToInt()),  // X 坐标
            static_cast<unsigned int>(valJson.at(1).ToInt())   // Y 坐标
        };
    }
}

// 移除字符串两端的空格
void trim(std::string& str) {
    // 移除开头的空格
    str = std::regex_replace(str, std::regex("^\\s+"), "");

    // 移除结尾的空格
    str = std::regex_replace(str, std::regex("\\s+$"), "");
}

/**
 * @function findEmojis
 * @brief 查找表情符号
 * @param cat: 表情符号分类
 * @param needlesStr: 关键字
 * @param results: 结果
 */
void EmojiDb::findEmojis(const std::string& cat, const std::string& needlesStr,
                         std::vector<const Emoji *>& results) const
{
    std::string catTrimmed {cat};  // 表情符号分类

    // split needles string into individual needles
    _tmpNeedles.clear();  // 关键字
    //boost::split(_tmpNeedles, needlesStr, boost::is_any_of(" "));

    std::istringstream iss(needlesStr); // 字符串输入流
    std::string needle; // 关键字

    while (std::getline(iss, needle, ' ')) {
        _tmpNeedles.push_back(needle);
    }

    if (_tmpNeedles.empty()) {
        // nothing to search
        return;
    }

    // trim category
    trim(catTrimmed);  // 移除两端的空格
    //boost::trim(catTrimmed);

    // this is to avoid duplicate entries in `results`
    _tmpFoundEmojis.clear();

    for (const auto& cat : _cats) {
        if (!catTrimmed.empty() && cat->lcName().find(catTrimmed) == std::string::npos) {
            // we don't want to search this category
            continue;
        }

        for (const auto& emoji : cat->emojis()) {
            bool select = true;

            for (const auto& keyword : emoji->keywords()) {
                select = true;

                for (const auto& needle : _tmpNeedles) {
                    if (needle.empty()) {
                        continue;
                    }

                    if (keyword.find(needle) == std::string::npos) {
                        // this keyword does not this needle
                        select = false;
                        break;
                    }
                }

                if (select) {
                    break;
                }
            }

            if (!select) {
                // not selected: next emoji
                continue;
            }

            if (_tmpFoundEmojis.find(emoji) != std::end(_tmpFoundEmojis)) {
                // we already have it: next emoji
                continue;
            }

            results.push_back(emoji);
            _tmpFoundEmojis.insert(emoji);
        }
    }
}

/**
 * @function recentEmojis
 * @brief 最近使用的表情符号
 * @param emojis: 表情符号
 */
void EmojiDb::recentEmojis(std::vector<const Emoji *>&& emojis)
{
    assert(_recentEmojisCat);
    _recentEmojisCat->emojis() = std::move(emojis);
}

/**
 * @function addRecentEmoji
 * @brief 添加最近使用的表情符号
 * @param emoji: 表情符号
 */
void EmojiDb::addRecentEmoji(const Emoji& emoji)
{
    assert(_recentEmojisCat);

    auto& emojis = _recentEmojisCat->emojis();

    while (true) {
        auto existingIt = std::find(std::begin(emojis), std::end(emojis), &emoji);

        if (existingIt == std::end(emojis)) {
            break;
        }

        emojis.erase(existingIt);
    }

    emojis.insert(std::begin(emojis), &emoji);

    constexpr auto maxRecentEmojis = 30U;

    if (emojis.size() > maxRecentEmojis) {
        emojis.resize(maxRecentEmojis);
    }
}

