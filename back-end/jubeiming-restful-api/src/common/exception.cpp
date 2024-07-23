// 异常实现
#include "exception.hpp"

using nlohmann::json;
jubeiming::utils::HTTPException::HTTPException(std::string_view msg, std::uint16_t code, const json& extra, std::int32_t status) :
    std::runtime_error(msg.data()),
    mMsg{ msg }, mCode{ code }, mExtra{ extra }, mStatus{ status }{}

jubeiming::utils::HTTPException& jubeiming::utils::HTTPException::operator=(const jubeiming::utils::HTTPException& rhs) {
    mMsg = rhs.mMsg;
    mCode = rhs.mCode;
    mExtra = rhs.mExtra;
    mStatus = rhs.mStatus;
    return *this;
}

jubeiming::utils::HTTPException::HTTPException(std::uint16_t code) : jubeiming::utils::HTTPException("", code) { };

jubeiming::utils::HTTPException::HTTPException(std::uint16_t code, const std::string& msg) : jubeiming::utils::HTTPException(msg, code) { }
jubeiming::utils::HTTPException::HTTPException(std::uint16_t code, const char* msg) : jubeiming::utils::HTTPException(msg, code) { }

jubeiming::utils::HTTPException::HTTPException(std::uint16_t code, std::int32_t status, const std::string& msg) : jubeiming::utils::HTTPException(msg, code, json(), status) { };

jubeiming::utils::HTTPException::HTTPException(std::uint16_t code, const json& extra) : jubeiming::utils::HTTPException("", code, extra) { };

jubeiming::utils::HTTPException::HTTPException(std::uint16_t code, std::int32_t status, const json& extra) : jubeiming::utils::HTTPException("", code, extra, status) { };

const std::string& jubeiming::utils::HTTPException::getMessage() const {
    return mMsg;
}

const std::uint16_t jubeiming::utils::HTTPException::getCode() const {
    return mCode;
}

const int jubeiming::utils::HTTPException::getStatus() const {
    return mStatus;
}

json jubeiming::utils::HTTPException::getJson() const {
    return mExtra;
}

const char* jubeiming::utils::HTTPException::what() const throw() {
    return mMsg.data();
}