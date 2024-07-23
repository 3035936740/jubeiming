/*
 * @File	  : mysql_conn.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Introduce : mysql连接器
*/

#pragma once

#include "boost/mysql/tcp.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/mysql/handshake_params.hpp"

#ifndef JUBEIMING_MYSQL_CONN_HPP
#define JUBEIMING_MYSQL_CONN_HPP

namespace jubeiming::utils {
	namespace {
		// 定义概念 is_unique_ptr
		template <typename T>
		concept UniquePtr = requires(T) {
			typename T::element_type;
			std::same_as<T, std::unique_ptr<typename T::element_type>>;
		};

		template <typename T>
		concept SharedPtr = requires(T) {
			typename T::element_type;
			std::same_as<T, std::shared_ptr<typename T::element_type>>;
		};

		// 工厂函数，根据类型生成相应的智能指针
		template <typename PtrType, typename... Args>
		PtrType make_pointer(Args&&... args) {
			if constexpr (UniquePtr<PtrType>) {
				return std::make_unique<typename PtrType::element_type>(std::forward<Args>(args)...);
			}
			else if constexpr (SharedPtr<PtrType>) {
				return std::make_shared<typename PtrType::element_type>(std::forward<Args>(args)...);
			}
			else {
				static_assert(std::is_same_v<PtrType, void>, "Unsupported pointer type");
			}
		}
	}

	class MySQLConnector {
	using MySQLConnects = std::unordered_map<std::string, std::shared_ptr<boost::mysql::tcp_connection>>;
	using ConnType = typename std::decay<decltype(std::declval<MySQLConnects>().begin()->second)>::type;
	private:
		boost::asio::io_context mCtx;
		boost::asio::ip::tcp::resolver mResolver;
		boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> mEndpoints;
		std::string mUsername, mPassword;
		MySQLConnects mMysqls;


	public:
		MySQLConnector() : mResolver{ mCtx.get_executor() } {}

		MySQLConnector(const std::string& host, const std::string& port, const std::string& username, const std::string& password) :
			mResolver{ mCtx.get_executor() },
			mUsername{ username },
			mPassword{ password } {
			mEndpoints = mResolver.resolve(host, port);
		};

		MySQLConnector& operator=(const MySQLConnector& rhs) {
			mEndpoints = rhs.mEndpoints;
			mUsername = rhs.mUsername;
			mPassword = rhs.mPassword;
			mMysqls = rhs.mMysqls;
			return *this;
		};

		void addDB(const std::string& db) {

			mMysqls[db] = make_pointer<ConnType>(boost::mysql::tcp_connection(mCtx.get_executor()));
			mMysqls[db]->connect(*mEndpoints.begin(), boost::mysql::handshake_params{
				mUsername,
				mPassword, 
				db
				});
		};

		ConnType getDB(const std::string& db) {
			return mMysqls.at(db);
		};

		~MySQLConnector() {
			for (auto& [key, conn] : mMysqls) {
				conn->close();
				conn.reset();
			}
		}
	};
}

#endif // JUBEIMING_MYSQL_CONN_HPP
