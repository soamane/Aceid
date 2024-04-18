#include "curlwrapper.h"

CurlWrapper* CurlWrapper::getInstance() {
    static CurlWrapper* Instance = new CurlWrapper();
    return Instance;
}

const curl_slist* CurlWrapper::addHeaders(std::initializer_list<std::string> headers) {
    curl_slist* headerList = nullptr;
    for (const auto& header : headers) {
        headerList = curl_slist_append(headerList, header.c_str());
        if (!headerList) {
            // TODO: error log
            return nullptr;
        }
    }
    return headerList;
}

const std::string CurlWrapper::performRequest(RequestType type, const std::string& source, const curl_slist* headers) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        // TODO: error log
        return std::string();
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, source.c_str());
    if (type == RequestType::eRT_HTTPS) {
        // TODO: HTTPS request type log
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    }

    if (headers != nullptr) {
        // TODO: headers nullptr!
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        // TODO: error log
        return std::string();
    }

    return response;
}

CurlWrapper::CurlWrapper() {
}

size_t CurlWrapper::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t total_size = size * nmemb;
    buffer->append((char*)contents, total_size);
    return total_size;
}
