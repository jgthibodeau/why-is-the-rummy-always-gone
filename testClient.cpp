#include <cstdlib>
#include <string>
#include <iostream>
#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>
#include <vector>

using namespace std;

int main(int argc, char** argv){
	string const serverUrl("http://localhost:8080/RPC2");
	string const methodName("server.do");

	xmlrpc_c::clientSimple myClient;
	xmlrpc_c::value result;

	myClient.call(serverUrl, methodName, "iiii", &result, 5,7,1,2);
	vector<xmlrpc_c::value> ret = xmlrpc_c::value_array(result).vectorValueValue();

	int i;
	for(std::vector<xmlrpc_c::value>::iterator it = ret.begin(); it!= ret.end(); it++){
		cout << (xmlrpc_c::value_int)(*it) << endl;
	}
	return 0;
}