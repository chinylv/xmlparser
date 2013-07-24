#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>
#include <unistd.h>
#include <sstream>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

using namespace std;

bool isNumber(const string str)
{
    unsigned short int digit;
    char ch;
    stringstream sstream(str);
    if (str.find("0") == 0 || !(sstream >> digit)
            || digit < 0 || sstream >> ch) {
        return false;
    }
    return true;
}

void findHelp() {
    cerr << "incorrect command, use ./rule -h to get help." << endl;
}

void printInformation()
{
    cout << "usage: ./rule [-f file -a action [-p port] [-t protocol]] | -h" << endl;
    cout << endl;
    cout << "The details about options & parameters:" << endl;
    cout << "   -f   file       " << "A must. Specify the input xml file." << endl;
    cout << "   -a   action     " << "A must. Specify permit or deny." << endl;
    cout << "   -p   port       " << "Not must. Specify the port number, 0~65535." << endl;
    cout << "   -t   protocol   " << "Not must. Specify the tcp or udp protocol." << endl;
    cout << "   -h              " << "Not must. Find help on how to use this command." << endl;
    cout << endl;
    cout << "You can sepcify the output file via pipeline. " << endl
        << "Like ./rule -f users.xml -p 20 -t tcp -a permit >> switches.rules"
        << endl << endl;
    cout << "For further information, please contact me at chinylv@gmail.com." << endl;
}

int main(int argc, char **argv)
{
    bool fFlag = false, tFlag = false, pFlag = false,
         aFlag = false, hFlag = false;
    string fParam, tParam, pParam, aParam;
    char ch;

    while ((ch = getopt(argc, argv, "f:t:p:a:h")) != -1) {
        switch (ch) {
            case 'f':
                if (fFlag) {
                    cerr << "multiple option -f" << endl;
                    return -1;
                }
                fFlag = true;
                fParam = string(optarg);
                if (fParam.rfind(".xml") == fParam.npos || fParam == ".xml" || fParam == "") {
                    cerr << "invalid input file, only xml files are allowed" << endl;
                    return -1;
                }
                break;
            case 't':
                if (tFlag) {
                    cerr << "multiple option -t" << endl;
                    return -1;
                }
                tFlag = true;
                tParam = string(optarg);
                if (tParam != "tcp" && tParam != "udp") {
                    cerr << "invalid protocol, only tcp or udp is allowed" << endl;
                    return -1;
                }
                break;
            case 'p':
                if (pFlag) {
                    cerr << "multiple option -p" << endl;
                    return -1;
                }
                pFlag = true;
                pParam = string(optarg);
                if (!isNumber(pParam)) {
                    cerr << "invalid port number" << endl;
                    return -1;
                }
                break;
            case 'a':
                if (aFlag) {
                    cerr << "multiple option -a" << endl;
                    return -1;
                }
                aFlag = true;
                aParam = string(optarg);
                if (aParam != "deny" && aParam != "permit") {
                    cerr << "invalid action, only deny or permit is allowed" << endl;
                    return -1;
                }
                break;
            case 'h':
                if (hFlag) {
                    cerr << "multiple option -h" << endl;
                    return -1;
                }
                hFlag = true;
                break;
            case '?':
                findHelp();
                return -1;
        }

    }

    if (hFlag && (fFlag || pFlag || tFlag || aFlag)) {
        findHelp();
        return -1;
    } else if (hFlag) {
       printInformation();
       return 1;
    }

    if (!fFlag) {
        cerr << "input files must be indicated, "
            << "use ./rule -h to get help."
            << endl;
        return -1;
    }
    if (!aFlag) {
        cerr << "action must be indicated, "
            << "use ./rule -h to get help."
            << endl;
        return -1;
    }

    xmlDocPtr doc;
    xmlNodePtr curNode;
    xmlNodePtr childNode;
    vector<string> iplist;

    xmlKeepBlanksDefault(0);
    doc = xmlReadFile(fParam.c_str(), "UTF-8", XML_PARSE_RECOVER); // open a xml doc.
    curNode = xmlDocGetRootElement(doc); // get root element.

    if (curNode == NULL) {
        cerr << "open file failed." << endl;
        xmlFreeDoc (doc);
        return -1;
    }


    if (xmlStrcmp(curNode->name, (const xmlChar*)"users")) {
        cerr << "check rootElement failed." << endl;
        xmlFreeDoc (doc);
        return -1;
    }
        
    string username;
    for (curNode = curNode->children; curNode != NULL; curNode = curNode->next) {
        username.clear();
        iplist.clear();
        for (childNode = curNode->children; childNode != NULL; childNode = childNode->next) {
            if (!xmlStrcmp(childNode->name, (const xmlChar*) "username") 
                    && xmlStrcmp(xmlNodeGetContent(childNode), (const xmlChar*) "")) {
                username = string((char*) xmlNodeGetContent(childNode));
            } else if (!xmlStrcmp(childNode->name, (const xmlChar*) "ip")
                    && xmlStrcmp(xmlNodeGetContent(childNode), (const xmlChar*) "")) {
                iplist.push_back(string((char*) xmlNodeGetContent(childNode)));
            }
        }
        if ("" != username && iplist.size() == 1) {
            cout << ((aParam == "permit") ? "entry PermitSSH" : "entry DenySSH")
                << username 
                << " { if match all { ";

            cout << (tFlag ? "protocol " + tParam + "; " : "");

            cout << "source-address "
                << iplist[0]
                << ";";
            
            cout << (pFlag ? (" destination-port " + pParam + ";") : "");

            cout << " } then { " 
                << aParam 
                << "; } }" 
                << endl;
        } else if ("" != username && iplist.size() > 1) {
            for (size_t cur = 0; cur < iplist.size(); cur++) {
                cout << ((aParam == "permit") ? "entry PermitSSH" : "entry DenySSH")
                    << username << setfill('0') << setw(2) << cur + 1
                    << " { if match all { ";

                cout << (tFlag ? ("protocol " + tParam + "; ") : "");

                cout << "source-address "
                    << iplist[cur]
                    << ";";
            
                cout << (pFlag ? (" destination-port " + pParam + ";") : "");

                cout << " } then { " 
                    << aParam 
                    << "; } }" 
                    << endl;
            }
        }
    }

    xmlFree(curNode);
    xmlFreeDoc(doc);
    return 1;
}
