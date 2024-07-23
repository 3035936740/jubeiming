#include "sensitive_words.hpp"
#include <fstream>
#include <queue>

jubeiming::utils::AhoCorasick::AhoCorasick() : root(std::make_unique<TrieNode>()) {}

void jubeiming::utils::AhoCorasick::insert(const std::string& word) {
        TrieNode* node = root.get();
        for (char ch : word) {
            if (!node->children.count(ch)) {
                node->children[ch] = std::make_unique<TrieNode>();
            }
            node = node->children[ch].get();
        }
        node->is_end_of_word = true;
    }

void jubeiming::utils::AhoCorasick::buildFailureLinks() {
        std::queue<TrieNode*> q;
        root->failure = root.get();
        for (auto& [ch, child] : root->children) {
            child->failure = root.get();
            q.push(child.get());
        }

        while (!q.empty()) {
            TrieNode* current = q.front();
            q.pop();

            for (auto& [ch, child] : current->children) {
                TrieNode* failure = current->failure;
                while (failure != root.get() && !failure->children.count(ch)) {
                    failure = failure->failure;
                }
                if (failure->children.count(ch)) {
                    child->failure = failure->children[ch].get();
                }
                else {
                    child->failure = root.get();
                }

                if (child->failure->is_end_of_word) {
                    child->is_end_of_word = true;
                }

                q.push(child.get());
            }
        }
    }

bool jubeiming::utils::AhoCorasick::search(const std::string& text) const {
        TrieNode* node = root.get();
        for (char ch : text) {
            while (node != root.get() && !node->children.count(ch)) {
                node = node->failure;
            }
            if (node->children.count(ch)) {
                node = node->children[ch].get();
            }
            if (node->is_end_of_word) {
                return true;
            }
        }
        return false;
    }

// 读取敏感词文件并将其存储在Aho-Corasick自动机中
void jubeiming::utils::loadSensitiveWords(const std::filesystem::path& filename, AhoCorasick& ac) {
    std::ifstream file(filename);
    std::string word;
    while (std::getline(file, word)) {
        if (!word.empty()) {
            ac.insert(word);
        }
    }
    ac.buildFailureLinks();
}