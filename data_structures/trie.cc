#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

class TrieNode {
 public:
  TrieNode(float w) : weight(w) {
    next.resize(26, nullptr);
  }

  float weight;  // < 0 means it is not the final node
  std::vector<TrieNode*> next;
};

class Trie {
 public:
  Trie() {
    root_ = new TrieNode(-1);
  }

  ~Trie() {
    release(root_);
  }

  void insert(const std::string& word, float weight) {
    TrieNode* it = root_;
    for (char ch : word) {
      size_t pos = ch - 'a';
      if (it->next[pos] == nullptr) {
        it->next[pos] = new TrieNode(-1);
      }
      it = it->next[pos];
    }
    it->weight = weight;
  }

  bool has(const std::string& word) {
    TrieNode* it = root_;
    for (auto ch : word) {
      size_t pos = ch - 'a';
      if (it->next[pos] == nullptr) {
        return false;
      }
      it = it->next[pos];
    }
    return it->weight > 0;
  }

  std::vector<std::pair<std::string, float>> list(const std::string& prefix) {
    TrieNode* it = root_;
    std::vector<std::pair<std::string, float>> words;
    for (char ch : prefix) {
      size_t pos = ch - 'a';
      if (it->next[pos] == nullptr) {
        return words;
      }
      it = it->next[pos];
    }

    dfs(it, prefix, words);

    return words;
  }

 public:
  void echo() {
    std::vector<std::pair<std::string, float>> allWords;
    dfs(root_, "", allWords);
    std::cout << "All inserted words are:" << std::endl;
    for (auto [word, weight] : allWords) {
      std::cout << word << " : " << weight << std::endl;
    }
  }

 private:
  void dfs(TrieNode* root, std::string word, std::vector<std::pair<std::string, float>>& words) {
    if (root->weight >= 0) {
      words.push_back({word, root->weight});
    }

    for (size_t pos = 0; pos < 26; ++pos) {
      auto nxt = root->next[pos];
      if (nxt != nullptr) {
        char ch = 'a' + pos;
        dfs(nxt, word + ch, words);
      }
    }
  }

  void release(TrieNode* root) {
    for (auto it : root->next) {
      if (it != nullptr) {
        release(it);
      }
    }

    delete root;
  }

 private:
  TrieNode* root_;
};

class DialPrompt {
 public:
  DialPrompt(std::vector<std::pair<std::string, float>>& wordTable) {
    for (auto& [word, weight] : wordTable) {
      dict_.insert(word, weight);
    }
    dict_.echo();  // for debug
  }

  std::vector<std::string> list(const std::string& numbers) {
    std::vector<std::string> prefixes;
    traveral(numbers, 0, "", prefixes);

    std::vector<std::pair<std::string, float>> wordWithWeights;
    for (auto& prefix : prefixes) {
      auto words = dict_.list(prefix);
      for (auto& word : words) {
        wordWithWeights.push_back(word);
      }
    }

    std::sort(wordWithWeights.begin(),
              wordWithWeights.end(),
              [](const std::pair<std::string, float>& a, const std::pair<std::string, float>& b) {
                return a.second > b.second;
              });

    std::vector<std::string> result;
    for (auto& [word, weight] : wordWithWeights) {
      result.push_back(word);
    }

    return result;
  }

 private:
  void traveral(const std::string& numbers,
                int pos,
                std::string prefix,
                std::vector<std::string>& prefixes) {
    if (pos == numbers.size()) {
      prefixes.push_back(prefix);
      return;
    }

    int number = numbers[pos] - '0';
    if (dialBoard.find(number) == dialBoard.end()) {
      return;
    }

    const std::string chs = dialBoard[number];
    for (auto const ch : chs) {
      traveral(numbers, pos + 1, prefix + ch, prefixes);
    }
  }

 private:
  inline static std::unordered_map<int, std::string> dialBoard = {{2, "abc"},
                                                                  {3, "def"},
                                                                  {4, "ghi"},
                                                                  {5, "jkl"},
                                                                  {6, "mno"},
                                                                  {7, "pqrs"},
                                                                  {8, "tuv"},
                                                                  {9, "wxyz"}};
  Trie dict_;
};

int main() {
  std::vector<std::pair<std::string, float>> wordTable = {{"qing", 5},
                                                          {"teng", 6},
                                                          {"mu", 7},
                                                          {"niao", 8},
                                                          {"za", 9},
                                                          {"ji", 10},
                                                          {"niu", 11},
                                                          {"qtmuniao", 12}};
  DialPrompt prompt(wordTable);

  std::cout << "Please enter the number:" << std::endl;
  std::string numbers;

  while (std::cin >> numbers) {
    auto result = prompt.list(numbers);
    std::cout << "the all candidates are:" << std::endl;
    for (auto word : result) {
      std::cout << word << std::endl;
    }
    std::cout << "Please enter the number:" << std::endl;
  }
};
