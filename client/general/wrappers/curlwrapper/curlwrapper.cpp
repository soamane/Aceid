﻿#include "curlwrapper.h"
#include "../../protect/xorstring/xorstring.h"

#include <stdexcept>

CurlWrapper* CurlWrapper::GetInstance() {
    static CurlWrapper* Instance = new CurlWrapper();
    return Instance;
}

const curl_slist* CurlWrapper::AddHeaders(std::initializer_list<std::string> headers) const {
    curl_slist* headerList = nullptr;
    for (const auto& header : headers) {
        headerList = curl_slist_append(headerList, header.c_str());
        if (!headerList) {
            throw std::runtime_error(xorstr_("Failed add headers"));
        }
    }
    return headerList;
}

const std::string CurlWrapper::PerformRequest(RequestType type, std::string_view source, const curl_slist* headers) const {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error(xorstr_("Failed init curl"));
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, source.data());
    if (type == RequestType::HTTPS) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    }

    if (headers != nullptr) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    curl_slist_free_all(const_cast<curl_slist*>(headers));
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        throw std::runtime_error(xorstr_("Failed to send request"));
    }

    return response;
}

CurlWrapper::CurlWrapper() { }

const std::size_t CurlWrapper::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t total_size = size * nmemb;
    buffer->append(reinterpret_cast<char*>(contents), total_size);
    return total_size;
}
