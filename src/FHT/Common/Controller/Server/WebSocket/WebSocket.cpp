/***************************************
*      Framework Handler Task
*  https://github.com/goganoga/FHT
*  Created: 29.11.19
*  Copyright (C) goganoga 2019
***************************************/
#include "WebSocket.h"
#include "Template.h"
#include <map>
#include <iostream>

bool wsRequest::parseWebsocketRequest(const char *s_req) {
    if (!s_req) {
        return false;
    }
    std::string str(s_req);
    std::map<std::string, std::string> map;
    std::string key;
    std::string value;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == '\r' && str[i + 1] == '\n') {
            map.emplace(key, value);
            ++i;
            key.clear();
            value.clear();
        } else if(str[i] == ':' && key.empty()){
            if (str[i+1] == ' ') {
                i++;
            }
            key = value;
            value.clear();
        } else {
        value += str[i];
        }
    }
    if(auto a = map_find(map, "Connection", "connection"); a != map.end()) {
        connection_ = a->second;
    }
    if (auto a = map_find(map, "Upgrade", "upgrade"); a != map.end()) {
        upgrade_ = a->second;
    }
    if (auto a = map_find(map, "Host", "host"); a != map.end()) {
        host_ = a->second;
    }
    if (auto a = map_find(map, "Origin", "origin"); a != map.end()) {
        origin_ = a->second;
    }
    if (auto a = map_find(map, "Cookie", "cookie"); a != map.end()) {
        cookie_ = a->second;
    }
    if (auto a = map_find(map, "Sec-WebSocket-Key", "sec-websocket-key"); a != map.end()) {
        secWebsocketKey_ = a->second;
    }
    if (auto a = map_find(map, "Sec-WebSocket-Version", "sec-websocket-version"); a != map.end()) {
        secWebsocketVersion_ = a->second;
    }

    return true;
}

std::string wsRequest::generateWebsocketResponse() {
    std::string resp;
    resp += "HTTP/1.1 101 WebSocket Protocol\r\n";
    resp += "Connection: Upgrade\r\n";
    resp += "Upgrade: websocket\r\n";
    resp += "Server: FHT Server\r\n";
    resp += "Sec-WebSocket-Accept: " + generateKey(secWebsocketKey_) + "\r\n";
    resp += "\r\n";
    return resp;
}

std::string wsRequest::generateKey(const std::string &key) {
    //sha-1
    std::string tmp = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    unsigned char md[SHA_DIGEST_LENGTH] = {0};
    SHA1((const unsigned char*)tmp.data(), tmp.size(), md);
    //base64 encode
    return base64_encode(md, 20);
}
