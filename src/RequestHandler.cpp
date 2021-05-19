#include "handlers.h"

void RequestHandler::wsHandle(socketIOmessageType_t type, byte* payload, size_t length, void* additional){
	RequestHandler* h = (RequestHandler*)additional;
	DynamicJsonDocument doc(2048);
	doc.to<JsonObject>();
	if(type == sIOtype_CONNECT){
		doc["requestID"] = "handshake";
		h->handleEvent("connected", doc.as<JsonObject>());
	}else if(type == sIOtype_DISCONNECT){
		h->handleEvent("disconnected", doc.as<JsonObject>());
		// lets do something evil >:)
		delay(15000);
		((WebSocketsClient*)&(h->sock))->disconnect();
	}else if(type == sIOtype_EVENT){
		deserializeJson(doc, (char*)payload);
		h->handleEvent(str(doc[0]), doc[1].as<JsonObject>());
	}
}

void RequestHandler::handleEvent(String event, JsonObject request){
	JsonArray out = request["final"].to<JsonArray>();
	out.add(request["requestID"]); // copy over requestID and free up
	request.remove("requestID");

	JsonObject response = request["result"].to<JsonObject>();

	for(eventmap& e : events){
		if(event == e.key){
			e.handler(request["data"], response);
			goto send;
		}
	}

	response["error"] = "invalid request!";

send:
	request.remove("data");
	out.add(request["result"]);
	request.remove("result");
	if(event == "disconnected") return; // temp fix
	String s;
	serializeJson(request["final"], s);
	sock.sendEVENT(s);

}

RequestHandler::RequestHandler(String host, short port){
	sock.begin(host, port);
	sock.additional = this;
	sock.onEvent(RequestHandler::wsHandle);
	registerAllEvents();
}