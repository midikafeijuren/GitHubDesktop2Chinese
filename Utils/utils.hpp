#ifndef UTILS_IS_EXITES
#define UTILS_IS_EXITES

#include <string>
#include <codecvt>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "http/httplib.h"

class utils {
public:
	
	static auto ansiToUtf8(const std::string& ansiString) -> std::string {
		int ansiSize = static_cast<int>(ansiString.size());
		int utf8Size = MultiByteToWideChar(CP_ACP, 0, ansiString.c_str(), ansiSize, nullptr, 0);
		std::vector<wchar_t> wideString(utf8Size);
		MultiByteToWideChar(CP_ACP, 0, ansiString.c_str(), ansiSize, wideString.data(), utf8Size);
		utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideString.data(), utf8Size, nullptr, 0, nullptr, nullptr);
		std::vector<char> utf8String(utf8Size);
		WideCharToMultiByte(CP_UTF8, 0, wideString.data(), utf8Size, utf8String.data(), utf8Size, nullptr, nullptr);
		return std::string(utf8String.begin(), utf8String.end());
	}

	static auto utf8ToAnsi(const std::string& utf8String) -> std::string {
		int utf8Size = static_cast<int>(utf8String.size());
		int ansiSize = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), utf8Size, nullptr, 0);
		std::vector<wchar_t> wideString(ansiSize);
		MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), utf8Size, wideString.data(), ansiSize);
		ansiSize = WideCharToMultiByte(CP_ACP, 0, wideString.data(), ansiSize, nullptr, 0, nullptr, nullptr);
		std::vector<char> ansiString(ansiSize);
		WideCharToMultiByte(CP_ACP, 0, wideString.data(), ansiSize, ansiString.data(), ansiSize, nullptr, nullptr);
		return std::string(ansiString.begin(), ansiString.end());
	}

	inline static auto to_wide_string(const std::string& input)-> std::wstring
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(input);
	}

	inline static auto to_byte_string(const std::wstring& input)->std::string
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(input);
	}

	static auto ReadFile(const std::string& filename)->std::string {
		std::ifstream fin(filename);
		std::stringstream buffer{};
		buffer << fin.rdbuf();
		std::string str(buffer.str());
		return str;
	}

	static auto WriteFile(const std::string& filename, std::string& txt) -> void {
		std::ofstream override (filename, std::ios::binary);
		override.write(txt.c_str(), txt.size());
		if (!override) {
			spdlog::error("�򿪲�д��Ŀ���ļ�:{} ʱʧ��", filename);
		}
		override.close();
	}

	static auto ReadHttpDataString(std::string url_host, std::string params, std::string& out) -> bool {
		httplib::Client cli(url_host);
		if (auto res = cli.Get(params)) {
			if (res->status == httplib::StatusCode::OK_200) {
				out = res->body;
				return true;
			}
			return false;
		}
		else {
			return false;
		}
	}

};


#endif // !UTILS_IS_EXITES