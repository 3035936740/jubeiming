#include "WXBizDataCrypt.h"

#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "openssl/aes.h"
#include "openssl/sha.h"
#include "openssl/evp.h"
#include <nlohmann/json.hpp>

#define FREE_PTR(ptr) \
    if (NULL != (ptr)) {\
        free (ptr);\
        (ptr) = NULL;\
    }

#define DELETE_PTR(ptr) \
    if (NULL != (ptr)) {\
        delete (ptr);\
        (ptr) = NULL;\
    }

void aes_decrypt(const unsigned char* key, const unsigned char* iv,
	const unsigned char* sSource, unsigned char* out, int iSize) {
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	int len;
	int plaintext_len;

	EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

	EVP_DecryptUpdate(ctx, out, &len, sSource, iSize);
	plaintext_len = len;

	EVP_DecryptFinal_ex(ctx, out + len, &len);
	plaintext_len += len;

	EVP_CIPHER_CTX_free(ctx);
}

namespace jubeiming::utils::WxBizDataSecure {

	int WXBizDataCrypt::DecryptData(
		const std::string& sEncryptedData,
		const std::string& sIv,
		std::string& sData)
	{
		std::string result;
		//1.decode base64 for sessionkey , iv and encryptedData
		std::string sAesData;
		if (0 != DecodeBase64(sEncryptedData, sAesData))
		{
			return WXBizDataCrypt_DecodeBase64_Error;
		}

		std::string sAesKey;
		if (0 != DecodeBase64(m_sSessionkey, sAesKey))
		{
			return WXBizDataCrypt_IllegalAesKey;
		}

		std::string sAesIv;
		if (0 != DecodeBase64(sIv, sAesIv))
		{
			return WXBizDataCrypt_IllegalIv;
		}

		//2.decode aes
		if (0 != AES_CBCDecrypt(sAesData, sAesKey, sAesIv, &result))
		{
			return WXBizDataCrypt_IllegalBuffer;
		}

		// 3. check appid
		// 使用 nlohmann 命名空间中的 json 类型
		nlohmann::json tJsonDoc;

		// 尝试解析 JSON 字符串
		if ((tJsonDoc = nlohmann::json::parse(result)) != nullptr) {
			// 检查 JSON 是否是对象
			if (!tJsonDoc.is_object()) {
				return WXBizDataCrypt_IllegalBuffer;
			}

			// 检查 "watermark" 键是否存在，并且对应的值是否是对象
			auto watermark = tJsonDoc.find("watermark");
			if (watermark == tJsonDoc.end() || !watermark->is_object()) {
				return WXBizDataCrypt_IllegalBuffer;
			}

			// 访问 "watermark" 对象
			auto& tWaterMarkDoc = *watermark;

			// 检查 "appid" 键是否存在，并且对应的值是否是字符串
			if (!tWaterMarkDoc.contains("appid") || !tWaterMarkDoc["appid"].is_string()) {
				return WXBizDataCrypt_IllegalBuffer;
			}

			// 比较 "appid" 值是否与预期的值匹配
			if (tWaterMarkDoc["appid"].get<std::string>() != m_sAppid) {
				return WXBizDataCrypt_IllegalBuffer;
			}
		}
		else {
			// 解析失败
			return WXBizDataCrypt_IllegalBuffer;
		}

		sData = result;
		return WXBizDataCrypt_OK;
	}


	int WXBizDataCrypt::AES_CBCDecrypt(const std::string& objSource,
		const std::string& objKey, const std::string& sIv,
		std::string* poResult)
	{
		return AES_CBCDecrypt(objSource.data(), objSource.size(),
			objKey.data(), objKey.size(),
			sIv.c_str(), sIv.size(),
			poResult);
	}

// 压制警告
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

	int WXBizDataCrypt::AES_CBCDecrypt(const char* sSource, const uint32_t iSize,
		const char* sKey, uint32_t iKeySize,
		const char* sIv, uint32_t iIvSize,
		std::string* poResult)
	{
		if (!sSource || !sKey || iSize < kAesKeySize || iSize % kAesKeySize != 0 || !poResult)
		{
			return -1;
		}

		poResult->clear();

		unsigned char* out = (unsigned char*)malloc(iSize);
		if (NULL == out)
		{
			return -1;
		}

		unsigned char key[kAesKeySize] = { 0 };
		unsigned char iv[kAesIVSize] = { 0 };
		memcpy(key, sKey, iKeySize > kAesKeySize ? kAesKeySize : iKeySize);
		memcpy(iv, sIv, iIvSize > kAesIVSize ? kAesIVSize : iIvSize);

		int iReturnValue = 0;
		AES_KEY aesKey;
		AES_set_decrypt_key(key, 8 * kAesKeySize, &aesKey);
		AES_cbc_encrypt((unsigned char*)sSource, out, iSize, &aesKey, iv, AES_DECRYPT);
		if (out[iSize - 1] > 0 && out[iSize - 1] <= kAesKeySize && (iSize - out[iSize - 1]) > 0)
		{
			poResult->append((char*)out, iSize - out[iSize - 1]);
		}
		else {
			iReturnValue = -1;
		}

		FREE_PTR(out);
		return iReturnValue;
	}

#pragma GCC diagnostic pop

	int WXBizDataCrypt::DecodeBase64(const std::string sSrc, std::string& sTarget)
	{
		if (0 == sSrc.size() || kMaxBase64Size < sSrc.size())
		{
			return -1;
		}

		//计算末尾=号个数
		int iEqualNum = 0;
		for (int n = sSrc.size() - 1; n >= 0; --n)
		{
			if (sSrc.c_str()[n] == '=')
			{
				iEqualNum++;
			}
			else
			{
				break;
			}
		}

		int iOutBufSize = sSrc.size();
		char* pcOutBuf = (char*)malloc(iOutBufSize);
		if (NULL == pcOutBuf)
		{
			return -1;
		}

		int iRet = 0;
		int iTargetSize = 0;
		iTargetSize = EVP_DecodeBlock((unsigned char*)pcOutBuf, (const unsigned char*)sSrc.c_str(), sSrc.size());
		if (iTargetSize > iEqualNum && iTargetSize < iOutBufSize)
		{
			sTarget.assign(pcOutBuf, iTargetSize - iEqualNum);
		}
		else
		{
			iRet = -1;
		}

		FREE_PTR(pcOutBuf);
		return iRet;
	}
}
