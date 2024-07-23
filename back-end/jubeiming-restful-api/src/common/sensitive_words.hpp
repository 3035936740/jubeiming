/*
 * @File	  : sensitive_words.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Introduce : 敏感字过滤
*/

#pragma once

#include <filesystem>
#include <unordered_map>
#include <memory>
#include <string>

#ifndef JUBEIMING_SENSITIVE_WORDS_HPP
#define JUBEIMING_SENSITIVE_WORDS_HPP

namespace jubeiming::utils {
    class AhoCorasick {
    public:
        AhoCorasick();
        void insert(const std::string&);
        void buildFailureLinks();
        bool search(const std::string&) const;
    private:
        struct TrieNode {
            std::unordered_map<char, std::unique_ptr<TrieNode>> children;
            TrieNode* failure = nullptr;
            bool is_end_of_word = false;
        };

        std::unique_ptr<TrieNode> root;
    };

    void loadSensitiveWords(const std::filesystem::path&, AhoCorasick&);
}

#endif // JUBEIMING_SENSITIVE_WORDS_HPP