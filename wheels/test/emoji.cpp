#include <wheels/test/emoji.hpp>

#include <wheels/support/random.hpp>

#include <vector>

std::string_view GetSuccessEmoji() {
  static const std::vector<std::string_view> kCandidates = {
      "(=^ェ^=)",
      "ヽ(´ー｀)ノ",
      "(ﾉ^_^)ﾉ",
      "~(˘▾˘~)",
  };
  return wheels::PickRandom(kCandidates);
}

std::string_view GetFailEmoji() {
  static const std::vector<std::string_view> kCandidates = {
      "¯\\_(ツ)_/¯",
      "(ノಠ益ಠ)ノ",
      "[¬º-°]¬",
      "ヽ(｀⌒´)ﾉ",
  };
  return wheels::PickRandom(kCandidates);
}
