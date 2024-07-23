/*
 * @File	  : exception.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Time      : 2023/03/05 21:15
 * @Introduce : 处理HTTP的异常类
*/

#pragma once

#include <exception>
#include <string>
#include <limits>
#include <string_view>
#include "nlohmann/json.hpp"

#ifndef JUBEIMING_EXCEPTION_HPP
#define JUBEIMING_EXCEPTION_HPP  

namespace jubeiming::utils {
    class HTTPException : public std::runtime_error {
    private:
        std::string mMsg;
        std::uint16_t mCode;
        std::int32_t mStatus;
        nlohmann::json mExtra;
    public:
        explicit HTTPException(std::string_view = "Severe HTTP Error", std::uint16_t = 500, const nlohmann::json & = nlohmann::json(), std::int32_t = std::numeric_limits<std::int32_t>::min());

        HTTPException& operator=(const HTTPException&);

        HTTPException(std::uint16_t);

        HTTPException(std::uint16_t, const std::string&);
        HTTPException(std::uint16_t, const char*);

        HTTPException(std::uint16_t, std::int32_t, const std::string&);

        HTTPException(std::uint16_t, const nlohmann::json&);

        HTTPException(std::uint16_t, std::int32_t, const nlohmann::json&);

        const std::string& getMessage() const;

        const std::uint16_t getCode() const;

        const int getStatus() const;

        nlohmann::json getJson() const;

        virtual const char* what() const throw() override;
    };
};

#endif // JUBEIMING_EXCEPTION_HPP