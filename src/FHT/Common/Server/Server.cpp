/***************************************
*      Framework Handler Task
*  https://github.com/goganoga/FHT
*  Created: 06.08.19
*  Copyright (C) goganoga 2019
***************************************/
#include "Server.h"
#include <evhttp.h>
namespace FHT {
	std::shared_ptr<iServer> iServer::Ininstanse = std::make_shared<Server>();
	bool Server::lessen_all = false;
	void Server::run() {
		try {
			/*void(*OnRequest)(evhttp_request *, void *) = [](evhttp_request *req, void *) {
				bool lessen_all_ = lessen_all;
				auto H = FHT::iConrtoller::hendlerManager;
				auto *OutBuf = evhttp_request_get_output_buffer(req);
				if (evhttp_request_get_command(req) == EVHTTP_REQ_GET || evhttp_request_get_command(req) == EVHTTP_REQ_POST) {
					if (!OutBuf) goto err;
					struct evkeyvalq headers;
					auto *InBuf = evhttp_request_get_input_buffer(req);
					auto LenBuf = evbuffer_get_length(InBuf);
					std::unique_ptr<char> postBody(new char[LenBuf + 1]);
					postBody.get()[LenBuf] = 0;
					evbuffer_copyout(InBuf, postBody.get(), LenBuf);
					evhttp_parse_query(evhttp_request_get_uri(req), &headers);
					auto location = evhttp_uri_get_path(evhttp_request_get_evhttp_uri(req));

					FHT::iHendler::data data_;
					data_.str0 = evhttp_request_get_uri(req); //uri
					data_.str1 = location ? location : ""; // location
					data_.obj2 = (void*)&headers; //headers
					data_.str2 = postBody.get(); //postBody
					data_.str3 = evhttp_request_get_host(req); //host
					data_.id = evhttp_uri_get_port(evhttp_request_get_evhttp_uri(req)); //port

					if (!location) goto err;

					auto func = H->getUniqueHendler(lessen_all_ ? "head" : location);
					if (!func) goto err;
					evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/plain; charset=utf-8");
					evbuffer_add_printf(OutBuf, func(data_).c_str());
					evhttp_clear_headers(&headers);
					evhttp_send_reply(req, HTTP_OK, "", OutBuf);
				}
				else {
				err:
					evbuffer_add_printf(OutBuf, "<html><body><center><h1>404</h1></center></body></html>");
					evhttp_send_reply(req, HTTP_NOTFOUND, "", OutBuf);
				}
			};*/
			initSer.reset(new InitSer(&Server::OnRequest, host, port));
		}
		catch (std::exception const &e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
	
void Server::OnRequest(evhttp_request *req, void *) {
	bool lessen_all_ = lessen_all;
	auto H = FHT::iConrtoller::hendlerManager;
	auto *OutBuf = evhttp_request_get_output_buffer(req);
	if (evhttp_request_get_command(req) == EVHTTP_REQ_GET || evhttp_request_get_command(req) == EVHTTP_REQ_POST) {
		if (!OutBuf) goto err;
		struct evkeyvalq headers;
		auto *InBuf = evhttp_request_get_input_buffer(req);
		auto LenBuf = evbuffer_get_length(InBuf);
		std::unique_ptr<char> postBody(new char[LenBuf + 1]);
		postBody.get()[LenBuf] = 0;
		evbuffer_copyout(InBuf, postBody.get(), LenBuf);
		evhttp_parse_query(evhttp_request_get_uri(req), &headers);
		auto location = evhttp_uri_get_path(evhttp_request_get_evhttp_uri(req));

		FHT::iHendler::data data_;
		data_.str0 = evhttp_request_get_uri(req); //uri
		data_.str1 = location ? location : ""; // location
		data_.obj2 = (void*)&headers; //headers
		data_.str2 = postBody.get(); //postBody
		data_.str3 = evhttp_request_get_host(req); //host
		data_.id = evhttp_uri_get_port(evhttp_request_get_evhttp_uri(req)); //port

		if (!location) goto err;

		auto func = H->getUniqueHendler(lessen_all_ ? "head" : location);
		if (!func) goto err;
		evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/plain; charset=utf-8");
		evbuffer_add_printf(OutBuf, func(data_).c_str());
		evhttp_clear_headers(&headers);
		evhttp_send_reply(req, HTTP_OK, "", OutBuf);
	}
	else {
	err:
		evbuffer_add_printf(OutBuf, "<html><body><center><h1>404</h1></center></body></html>");
		evhttp_send_reply(req, HTTP_NOTFOUND, "", OutBuf);
	}
	}
	
	//return Hendler lessen name
	std::string Server::lessenAll(bool flag) {
		lessen_all = flag;
		return flag ? "head" : "";
	}
	void Server::setPort(std::uint16_t port_) {
		port = port_;
	}
	void Server::setHost(std::string host_) {
		host = host_;
	}
}