#pragma once

#include <vector>
#include <string>
#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "simple-json.hpp"

enum class EmojiVersion
{
    V_0_6,
    V_0_7,
    V_1_0,
    V_2_0,
    V_3_0,
    V_4_0,
    V_5_0,
    V_11_0,
    V_12_0,
    V_12_1,
    V_13_0,
    V_13_1,
    V_14_0,
};

class Emoji
{
public:
    using Codepoint = char32_t;                  // 代码点
    using Codepoints = std::vector<Codepoint>;   // 代码点集合

public:
    enum class SkinTone  // 肤色
    {
        LIGHT,            // 浅色
        MEDIUM_LIGHT,     // 中浅色
        MEDIUM,           // 中等
        MEDIUM_DARK,      // 中深色
        DARK,             // 深色
    };

public:
    explicit Emoji(const std::string& str, const std::string& name,
                   std::unordered_set<std::string>&& keywords, bool hasSkinToneSupport,
                   EmojiVersion version);
    Codepoints codepoints() const;                              // 代码点
    Codepoints codepointsWithSkinTone(SkinTone skinTone) const; // 代码点和肤色
    std::string strWithSkinTone(SkinTone skinTone) const;       
    const std::string& lcName() const;                         // 名称

    const std::string& str() const noexcept
    {
        return _str;
    }

    const std::string& name() const noexcept
    {
        return _name;
    }

    const std::unordered_set<std::string>& keywords() const noexcept
    {
        return _keywords;
    }

    bool hasSkinToneSupport() const noexcept
    {
        return _hasSkinToneSupport;
    }

    EmojiVersion version() const noexcept
    {
        return _version;
    }

private:
    const std::string _str;          // 字符串
    const std::string _name;         // 名称
    mutable std::string _lcName;     // 小写名称
    const std::unordered_set<std::string> _keywords; // 关键字
    const bool _hasSkinToneSupport;  // 是否支持肤色
    const EmojiVersion _version;     // 版本
};

// 
class EmojiCat
{
public:
    explicit EmojiCat(const std::string& id, const std::string& name);
    explicit EmojiCat(const std::string& id, const std::string& name,
                      std::vector<const Emoji *>&& emojis);
    const std::string& lcName() const;

    const std::string& id() const noexcept
    {
        return _id;
    }

    const std::string& name() const noexcept
    {
        return _name;
    }

    std::vector<const Emoji *>& emojis() noexcept
    {
        return _emojis;
    }

    const std::vector<const Emoji *>& emojis() const noexcept
    {
        return _emojis;
    }

    const std::string& icon() const noexcept
    {
		return _icon;
	}

    void setIcon(const std::string& icon)
    {
		_icon = icon;
	}

private:
    const std::string _id;
    const std::string _name;
    std::string _icon {};
    mutable std::string _lcName;
    std::vector<const Emoji *> _emojis;
};

struct EmojisPngLocation  // 表情符号 PNG 位置
{
    unsigned int x;
    unsigned int y;
};

class EmojiDb
{
public:
    enum class EmojiSize  // 表情符号大小
    {
        SIZE_16 = 16,
        SIZE_24 = 24,
        SIZE_32 = 32,
        SIZE_40 = 40,
        SIZE_48 = 48,
    };

public:
    //explicit EmojiDb(const std::string& dir, EmojiSize emojiSize);
    explicit EmojiDb();
    void init(const std::string& dir, const EmojiSize emojiSize);

    void findEmojis(const std::string& cat, const std::string& needles,
                    std::vector<const Emoji *>& results) const;
    void recentEmojis(std::vector<const Emoji *>&& emojis);
    void addRecentEmoji(const Emoji& emoji);

    EmojiSize emojiSize() const
    {
        return _emojiSize;
    }

    unsigned int emojiSizeInt() const
    {
        return static_cast<unsigned int>(_emojiSize);
    }

    const std::string& emojisPngPath() const noexcept
    {
        return _emojisPngPath;
    }

    const std::vector<std::unique_ptr<EmojiCat>>& cats() const noexcept
    {
        return _cats;
    }

    const std::unordered_map<std::string, std::unique_ptr<const Emoji>>& emojis() const noexcept
    {
        return _emojis;
    }

    const EmojiCat& recentEmojisCat() const noexcept
    {
        return *_recentEmojisCat;
    }

    const Emoji& emojiForStr(const std::string& str) const
    {
        return *_emojis.at(str);
    }

    const std::unordered_set<std::string>& keywords() const noexcept
    {
        return _keywords;
    }

    const std::unordered_map<const Emoji *, EmojisPngLocation>& emojiPngLocations() const noexcept
    {
        return _emojiPngLocations;
    }

    const std::unordered_set<const Emoji *>& emojisForKeyword(const std::string& keyword) const
    {
        return _keywordEmojis.find(keyword)->second;
    }

private:
    json::JSON _loadJson(const std::string& dir, const std::string& file);
    void _createEmojis(const std::string& dir);
    void _createCats(const std::string& dir);
    void _createEmojiPngLocations(const std::string& dir);

private:
    EmojiSize _emojiSize;        // 表情符号大小
    std::string _emojisPngPath;  // 表情符号 PNG 路径
    std::vector<std::unique_ptr<EmojiCat>> _cats;  // 表情符号分类
    std::unordered_map<std::string, std::unique_ptr<const Emoji>> _emojis;  // 表情符号
    std::unordered_map<std::string, std::unordered_set<const Emoji *>> _keywordEmojis;  // 关键字表情符号
    std::unordered_set<std::string> _keywords; // 关键字
    std::unordered_map<const Emoji *, EmojisPngLocation> _emojiPngLocations; // 表情符号 PNG 位置
    mutable std::vector<std::string> _tmpNeedles; // 临时针
    mutable std::unordered_set<const Emoji *> _tmpFoundEmojis; // 临时找到的表情符号
    EmojiCat *_recentEmojisCat = nullptr;  // 最近的表情符号分类

    bool _initialized = false;  // 是否初始化
};

